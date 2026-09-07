#include <nitro/fs.h>

extern FsArchive *arc_list;
extern FsDirectoryPosition current_dir_pos;

int FSi_ReadMemCallback(FsArchive *archive, void *buffer, u32 offset, u32 size) {
    MI_CpuCopy8((void *)(archive->base + offset), buffer, size);
    return 0;
}
int FSi_WriteMemCallback(FsArchive *archive, void *buffer, u32 offset, u32 size) {
    MI_CpuCopy8(buffer, (void *)(archive->base + offset), size);
    return 0;
}
int FSi_ReadMemoryCore(FsArchive *archive, void *buffer, u32 offset, u32 size) {
    MI_CpuCopy8((void *)offset, buffer, size);
    return 0;
}

FsArchive *FS_FindArchive(const char *name, int length) {
    u32 packed = FSi_GetPackedName(name, length);
    u32 state = OS_DisableInterrupts();
    FsArchive *archive;
    for (archive = arc_list; archive && archive->name != packed; archive = archive->next) {}
    OS_RestoreInterrupts(state);
    return archive;
}

FsFile *FSi_NextCommand(FsArchive *archive) {
    u32 state = OS_DisableInterrupts();
    FsFile *file;
    if (FSi_TestArchiveFlag(archive, 0x20)) {
        archive->flags &= ~0x20;
        file = (FsFile *)archive->commands.next;
        while (file) {
            FsFile *next = (FsFile *)file->link.next;
            if (FSi_TestFileFlag(file, 2)) {
                if ((FsFile *)archive->commands.next == file) archive->commands.next = (FsLink *)next;
                FSi_ReleaseCommand(file, 3);
                if (!next) next = (FsFile *)archive->commands.next;
            }
            file = next;
        }
    }
    if (!FSi_TestArchiveFlag(archive, 0x40) && !FSi_TestArchiveFlag(archive, 8) &&
        (file = (FsFile *)archive->commands.next)) {
        int idle = FSi_TestArchiveFlag(archive, 0x10) == 0;
        if (idle) archive->flags |= 0x10;
        OS_RestoreInterrupts(state);
        if (idle && (archive->procedure_mask & 0x200)) archive->procedure(file, 9);
        state = OS_DisableInterrupts();
        file->flags |= 0x40;
        if (FSi_TestFileFlag(file, 4)) {
            OS_WakeupThread(&file->queue);
            OS_RestoreInterrupts(state);
            return 0;
        }
        OS_RestoreInterrupts(state);
        return file;
    } else {
        if (FSi_TestArchiveFlag(archive, 0x10)) {
            archive->flags &= ~0x10;
            if (archive->procedure_mask & 0x400) {
                FsFile temporary;
                FS_InitFile(&temporary);
                temporary.archive = archive;
                archive->procedure(&temporary, 10);
            }
        }
        if (FSi_TestArchiveFlag(archive, 0x40)) {
            archive->flags &= ~0x40;
            archive->flags |= 8;
            OS_WakeupThread(&archive->suspend_queue);
        }
        OS_RestoreInterrupts(state);
        return 0;
    }
}

void FSi_ExecuteAsyncCommand(FsFile *file) {
    FsArchive *archive = file->archive;
    while (file) {
        u32 state = OS_DisableInterrupts();
        file->flags |= 0x40;
        if (FSi_TestFileFlag(file, 4)) {
            OS_WakeupThread(&file->queue);
            OS_RestoreInterrupts(state);
            break;
        }
        file->flags |= 8;
        OS_RestoreInterrupts(state);
        if (func_0203e118(file, file->command) == 6) break;
        file = FSi_NextCommand(archive);
    }
}

int FSi_ExecuteSyncCommand(FsFile *file) {
    int result = func_0203e118(file, file->command);
    FsFile *next;
    FSi_ReleaseCommand(file, result);
    next = FSi_NextCommand(file->archive);
    if (next) FSi_ExecuteAsyncCommand(next);
    return file->result == 0;
}

int FSi_SendCommand(FsFile *file, int command) {
    FsArchive *archive = file->archive;
    u32 mask = 1 << command;
    u32 state;
    file->command = command;
    file->result = 2;
    file->flags |= 1;
    state = OS_DisableInterrupts();
    if (archive->flags & 0x80) {
        FSi_ReleaseCommand(file, 3);
        OS_RestoreInterrupts(state);
        return 0;
    }
    if (mask & 0x1FC) file->flags |= 4;
    FSi_AppendToList(&file->link, &archive->commands);
    if (!FSi_TestArchiveFlag(archive, 8) && !FSi_TestArchiveFlag(archive, 0x10)) {
        archive->flags |= 0x10;
        OS_RestoreInterrupts(state);
        if (archive->procedure_mask & 0x200) archive->procedure(file, 9);
        state = OS_DisableInterrupts();
        file->flags |= 0x40;
        if (!FSi_TestFileFlag(file, 4)) {
            OS_RestoreInterrupts(state);
            FSi_ExecuteAsyncCommand(file);
            return 1;
        }
        OS_RestoreInterrupts(state);
    } else {
        if (!FSi_TestFileFlag(file, 4)) {
            OS_RestoreInterrupts(state);
            return 1;
        }
        do { OS_SleepThread(&file->queue); } while (!(file->flags & 0x40));
        OS_RestoreInterrupts(state);
    }
    return FSi_ExecuteSyncCommand(file);
}

void FS_NotifyArchiveAsyncEnd(FsArchive *archive, int result) {
    if (FSi_TestArchiveFlag(archive, 0x100)) {
        FsFile *file = (FsFile *)archive->commands.next;
        archive->flags &= ~0x100;
        FSi_ReleaseCommand(file, result);
        file = FSi_NextCommand(archive);
        if (file) FSi_ExecuteAsyncCommand(file);
    } else {
        FsFile *file = (FsFile *)archive->commands.next;
        u32 state = OS_DisableInterrupts();
        file->result = result;
        archive->flags &= ~0x200;
        OS_WakeupThread(&archive->sync_queue);
        OS_RestoreInterrupts(state);
    }
}

void FS_InitArchive(FsArchive *archive) {
    MI_CpuFill8(archive, 0, sizeof(*archive));
    archive->sync_queue = 0;
    archive->suspend_queue = 0;
}

int FS_RegisterArchiveName(FsArchive *archive, const char *name, int length) {
    int registered = 0;
    u32 state = OS_DisableInterrupts();
    if (!FS_FindArchive(name, length)) {
        FsArchive *tail = arc_list;
        if (!tail) {
            arc_list = archive;
            current_dir_pos.archive = archive;
            current_dir_pos.position = 0;
            current_dir_pos.index = 0;
            current_dir_pos.directory = 0;
        } else {
            while (tail->next) tail = tail->next;
            tail->next = archive;
            archive->previous = tail;
        }
        archive->name = FSi_GetPackedName(name, length);
        archive->flags |= 1;
        registered = 1;
    }
    OS_RestoreInterrupts(state);
    return registered;
}

int FS_LoadArchive(FsArchive *archive, u32 base, u32 fat, u32 fat_size,
                   u32 fnt, u32 fnt_size, FsArchiveIo read, FsArchiveIo write) {
    archive->base = base;
    archive->fat_size = fat_size;
    archive->original_fat = fat;
    archive->fat = archive->original_fat;
    archive->fnt_size = fnt_size;
    archive->original_fnt = fnt;
    archive->fnt = archive->original_fnt;
    archive->read = read ? read : FSi_ReadMemCallback;
    archive->write = write ? write : FSi_WriteMemCallback;
    archive->table_read = archive->read;
    archive->table_buffer = 0;
    archive->flags |= 2;
    return 1;
}

