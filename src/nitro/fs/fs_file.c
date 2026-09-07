#include <nitro/fs.h>

extern FsDirectoryPosition current_dir_pos;
extern u32 is_init;
extern void FSi_InitRom(int dma);

void FSi_CutFromList(FsLink *item) {
    FsLink *previous = item->previous;
    FsLink *next = item->next;
    if (previous) previous->next = next;
    if (next) next->previous = previous;
    item->previous = 0;
    item->next = item->previous;
}

void FSi_AppendToList(FsLink *item, FsLink *list) {
    FsLink *previous = item->previous;
    FsLink *next = item->next;
    if (previous) previous->next = next;
    if (next) next->previous = previous;
    while (list->next) list = list->next;
    list->next = item;
    item->previous = list;
    item->next = 0;
}

void FS_Init(int dma) {
    if (is_init) return;
    is_init = 1;
    FSi_InitRom(dma);
}

void FS_InitFile(FsFile *file) {
    file->link.previous = 0;
    file->link.next = file->link.previous;
    file->queue = 0;
    file->archive = 0;
    file->command = 14;
    file->flags = 0;
}

int FSi_FindPath(FsFile *file, const char *path, FsFileId *id, FsDirectoryPosition *directory) {
    FsDirectoryPosition position;
    int index;
    if ((u8)*path == '/' || (u8)*path == '\\') {
        position.archive = current_dir_pos.archive;
        position.directory = 0;
        position.position = 0;
        position.index = 0;
        path++;
    } else {
        position = current_dir_pos;
        for (index = 0; index <= 3; index++) {
            int character = (u8)path[index];
            if (!character || character == '/' || character == '\\') break;
            if (character == ':') {
                FsArchive *archive = FS_FindArchive(path, index);
                if (!archive) return 0;
                if (!FSi_TestArchiveFlag(archive, 2)) return 0;
                position.archive = archive;
                position.position = 0;
                position.index = 0;
                position.directory = 0;
                path += index + 1;
                if ((u8)*path == '/' || (u8)*path == '\\') path++;
                break;
            }
        }
    }
    file->archive = position.archive;
    file->argument.path.path = path;
    file->argument.path.directory = position;
    if (directory) {
        file->argument.path.find_directory = 1;
        file->argument.path.output = directory;
    } else {
        file->argument.path.find_directory = 0;
        file->argument.path.output = id;
    }
    return FSi_SendCommand(file, 4);
}

int FSi_ReadFileCore(FsFile *file, void *buffer, int size, int asynchronous) {
    int position = file->property.file.position;
    int requested = size;
    int remaining = file->property.file.bottom - position;
    file->argument.io.buffer = buffer;
    if (size > remaining) size = remaining;
    if (size < 0) size = 0;
    file->argument.io.size = requested;
    file->argument.io.length = size;
    if (!asynchronous) file->flags |= 4;
    FSi_SendCommand(file, 0);
    if (!asynchronous) {
        if (FS_WaitAsync(file)) size = file->property.file.position - position;
        else size = -1;
    }
    return size;
}

int FS_ConvertPathToFileID(FsFileId *id, const char *path) {
    FsFile file;
    FS_InitFile(&file);
    return FSi_FindPath(&file, path, id, 0) != 0;
}

int FS_OpenFileDirect(FsFile *file, FsArchive *archive, u32 top, u32 bottom, u32 id) {
    file->archive = archive;
    file->argument.direct.id = id;
    file->argument.direct.top = top;
    file->argument.direct.bottom = bottom;
    if (!FSi_SendCommand(file, 7)) return 0;
    file->flags |= 0x10;
    file->flags &= ~0x20;
    return 1;
}

int FS_OpenFileFast(FsFile *file, FsFileId id) {
    if (!id.archive) return 0;
    file->archive = id.archive;
    file->argument.file_id = id;
    if (!FSi_SendCommand(file, 6)) return 0;
    file->flags |= 0x10;
    file->flags &= ~0x20;
    return 1;
}

int FS_OpenFile(FsFile *file, const char *path) {
    FsFileId id;
    return FS_ConvertPathToFileID(&id, path) && FS_OpenFileFast(file, id);
}

int FS_CloseFile(FsFile *file) {
    if (!FSi_SendCommand(file, 8)) return 0;
    file->archive = 0;
    file->command = 14;
    file->flags &= ~0x30;
    return 1;
}

int FS_WaitAsync(FsFile *file) {
    int execute = 0;
    u32 state = OS_DisableInterrupts();
    if (FS_IsBusy(file)) {
        execute = (file->flags & 0x44) == 0;
        if (execute) {
            file->flags |= 4;
            do { OS_SleepThread(&file->queue); } while (!(file->flags & 0x40));
        } else {
            do { OS_SleepThread(&file->queue); } while (FS_IsBusy(file));
        }
    }
    OS_RestoreInterrupts(state);
    if (execute) return FSi_ExecuteSyncCommand(file);
    return file->result == 0;
}

void FS_CancelFile(FsFile *file) {
    u32 state = OS_DisableInterrupts();
    if (FS_IsBusy(file)) {
        file->flags |= 2;
        file->archive->flags |= 0x20;
    }
    OS_RestoreInterrupts(state);
}

int FS_ReadFileAsync(FsFile *file, void *buffer, int size) { return FSi_ReadFileCore(file, buffer, size, 1); }
int FS_ReadFile(FsFile *file, void *buffer, int size) { return FSi_ReadFileCore(file, buffer, size, 0); }

int FS_SeekFile(FsFile *file, int offset, int origin) {
    switch (origin) {
    case 0: offset += file->property.file.top; break;
    case 1: offset += file->property.file.position; break;
    case 2: offset += file->property.file.bottom; break;
    default: return 0;
    }
    if (offset < file->property.file.top) offset = file->property.file.top;
    if (offset > file->property.file.bottom) offset = file->property.file.bottom;
    file->property.file.position = offset;
    return 1;
}
