#include <nitro/fs.h>

extern int (*data_0204ff08[])(FsFile *file);

int FS_SuspendArchive(FsArchive *archive) {
    u32 state = OS_DisableInterrupts();
    int running = FSi_TestArchiveFlag(archive, 8) == 0;
    if (running) {
        if (FSi_TestArchiveFlag(archive, 0x10)) {
            archive->flags |= 0x40;
            do { OS_SleepThread(&archive->suspend_queue); } while (FSi_TestArchiveFlag(archive, 0x40));
        } else archive->flags |= 8;
    }
    OS_RestoreInterrupts(state);
    return running;
}

int FS_ResumeArchive(FsArchive *archive) {
    FsFile *file = 0;
    u32 state = OS_DisableInterrupts();
    int running = FSi_TestArchiveFlag(archive, 8) == 0;
    if (!running) {
        archive->flags &= ~8;
        file = FSi_NextCommand(archive);
    }
    OS_RestoreInterrupts(state);
    if (file) FSi_ExecuteAsyncCommand(file);
    return running;
}

void FS_SetArchiveProc(FsArchive *archive, FsArchiveProc procedure, u32 mask) {
    if (!mask) procedure = 0;
    else if (!procedure) mask = 0;
    archive->procedure = procedure;
    archive->procedure_mask = mask;
}

void FSi_ReleaseCommand(FsFile *file, int result) {
    u32 state = OS_DisableInterrupts();
    FSi_CutFromList(&file->link);
    file->flags &= ~0x4F;
    file->result = result;
    OS_WakeupThread(&file->queue);
    OS_RestoreInterrupts(state);
}

int func_0203e118(FsFile *file, int command) {
    FsArchive *archive = file->archive;
    int result;
    u32 mask = 1 << command;
    if (FSi_TestFileFlag(file, 4)) archive->flags |= 0x200;
    else archive->flags |= 0x100;
    if (archive->procedure_mask & mask) {
        result = archive->procedure(file, command);
        switch (result) {
        case 0: case 1: case 4:
            file->result = result;
            break;
        case 8:
            archive->procedure_mask &= ~mask;
            result = 7;
            break;
        case 2: case 3: case 5: case 6: case 7: default:
            break;
        }
    } else result = 7;
    if (result == 7) result = data_0204ff08[command](file);
    if (result == 6) {
        if (FSi_TestFileFlag(file, 4)) {
            u32 state = OS_DisableInterrupts();
            while (FSi_TestArchiveFlag(archive, 0x200)) OS_SleepThread(&archive->sync_queue);
            result = file->result;
            OS_RestoreInterrupts(state);
        }
    } else if (FSi_TestFileFlag(file, 4)) {
        archive->flags &= ~0x200;
        file->result = result;
    } else {
        archive->flags &= ~0x100;
        FSi_ReleaseCommand(file, result);
    }
    return result;
}

void func_0203e068(FsReadCursor *cursor, void *buffer, u32 size) {
    FsArchive *archive = cursor->archive;
    int result;
    archive->flags |= 0x200;
    result = archive->table_read(archive, buffer, cursor->position, size);
    switch (result) {
    case 0: case 1:
        archive->flags &= ~0x200;
        break;
    case 6: {
        u32 state = OS_DisableInterrupts();
        while (FSi_TestArchiveFlag(archive, 0x200)) OS_SleepThread(&archive->sync_queue);
        OS_RestoreInterrupts(state);
        break;
    }
    }
    cursor->position += size;
}

int func_0203e034(FsFile *file, u32 directory) {
    file->flags |= 4;
    file->argument.directory.archive = file->archive;
    file->argument.directory.position = 0;
    file->argument.directory.index = 0;
    file->argument.directory.directory = directory;
    return func_0203e118(file, 2);
}

int func_0203dfd0(const u8 *left, const u8 *right, int length) {
    int index;
    for (index = 0; index < length; index++) {
        u32 left_value = left[index] - 'A';
        u32 right_value = right[index] - 'A';
        if (left_value <= 'Z' - 'A') left_value += 'a' - 'A';
        if (right_value <= 'Z' - 'A') right_value += 'a' - 'A';
        if (left_value != right_value) return left_value - right_value;
    }
    return 0;
}

int func_0203df98(FsFile *file) {
    u32 position = file->property.file.position;
    u32 length = file->argument.io.length;
    FsArchive *archive = file->archive;
    void *buffer = file->argument.io.buffer;
    file->property.file.position += length;
    return archive->read(archive, buffer, position, length);
}

int func_0203df60(FsFile *file) {
    u32 position = file->property.file.position;
    u32 length = file->argument.io.length;
    FsArchive *archive = file->archive;
    void *buffer = file->argument.io.buffer;
    file->property.file.position += length;
    return archive->write(archive, buffer, position, length);
}

int func_0203dec8(FsFile *file) {
    FsArchive *archive = file->archive;
    FsDirectoryPosition *position = &file->argument.directory;
    struct { u32 offset; u16 first_file; u16 parent; } entry;
    FsReadCursor cursor;
    cursor.archive = archive;
    cursor.position = archive->fnt + 8 * position->directory;
    func_0203e068(&cursor, &entry, 8);
    file->property.directory.position = *position;
    if (!position->index && !position->position) {
        file->property.directory.position.index = entry.first_file;
        file->property.directory.position.position = archive->fnt + entry.offset;
    }
    file->property.directory.parent = entry.parent & 0xFFF;
    return 0;
}

int func_0203ddb8(FsFile *file) {
    FsDirectoryEntry *entry = (FsDirectoryEntry *)file->argument.words[0];
    u8 length_and_type;
    u16 directory;
    FsReadCursor cursor;
    cursor.archive = file->archive;
    cursor.position = file->property.directory.position.position;
    func_0203e068(&cursor, &length_and_type, 1);
    entry->name_length = length_and_type & 0x7F;
    entry->is_directory = (length_and_type >> 7) & 1;
    if (!entry->name_length) return 1;
    if (file->argument.words[1]) cursor.position += entry->name_length;
    else {
        func_0203e068(&cursor, entry->name, entry->name_length);
        entry->name[entry->name_length] = 0;
    }
    if (entry->is_directory) {
        func_0203e068(&cursor, &directory, 2);
        entry->entry.directory.archive = file->archive;
        entry->entry.directory.directory = directory & 0xFFF;
        entry->entry.directory.index = 0;
        entry->entry.directory.position = 0;
    } else {
        entry->entry.file.archive = file->archive;
        entry->entry.file.id = file->property.directory.position.index;
        file->property.directory.position.index++;
    }
    file->property.directory.position.position = cursor.position;
    return 0;
}
