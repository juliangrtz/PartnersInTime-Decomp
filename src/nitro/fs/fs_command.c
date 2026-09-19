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

static inline int FSi_IsPathCharacter(u32 character)
{
    int valid = 0;
    if (character && character != '/' && character != '\\') valid = 1;
    return valid;
}

int FSi_FindPathCommand(FsFile *file)
{
    FsDirectoryEntry entry;
    int find_directory = file->argument.path.find_directory;
    const u8 *path = (const u8 *)file->argument.path.path;
    func_0203e118(file, 2);
    while (*path) {
        int directory;
        int length;
        for (length = 0; FSi_IsPathCharacter(directory = path[length]); ++length) {}
        if (directory || find_directory) directory = 1;
        if (!length) return 1;
        if (*path == '.') {
            if (length == 1) {
                ++path;
                goto advance;
            }
            if ((length == 2) & (path[1] == '.')) {
                if (file->property.directory.position.directory)
                    func_0203e034(file, file->property.directory.parent);
                path += 2;
                goto advance;
            }
        }
        if (length > 127) return 1;
        file->argument.read_directory.entry = &entry;
        file->argument.read_directory.skip_name = 0;
        do {
            if (func_0203e118(file, 3)) return 1;
        } while (directory != entry.is_directory || length != entry.name_length
            || func_0203dfd0(path, (const u8 *)entry.name, length));
        if (directory) {
            file->argument.directory = entry.entry.directory;
            path += length;
            func_0203e118(file, 2);
        } else {
            if (find_directory) return 1;
            *(FsFileId *)file->argument.path.output = entry.entry.file;
            return 0;
        }
advance:
        path += *path != 0;
    }
    if (!find_directory) return 1;
    *(FsDirectoryPosition *)file->argument.path.output = file->property.directory.position;
    return 0;
}

extern const char data_02059d94[];

int FSi_GetPathCommand(FsFile *file)
{
    FsArchive *archive = file->archive;
    FsGetPathArgs *request = &file->argument.get_path;
    FsDirectoryEntry entry;
    FsFile search;
    u32 directory;
    u32 file_id;
    char *buffer;
    u32 length;
    FS_InitFile(&search);
    search.archive = file->archive;
    if (FSi_TestFileFlag(file, 0x20)) {
        directory = file->property.directory.position.directory;
        file_id = 0x10000;
    } else {
        file_id = file->property.file.id;
        if (request->length)
            directory = request->directory;
        else {
            u32 index = 0;
            u32 count = 0;
            directory = 0x10000;
            do {
                func_0203e034(&search, index);
                if (!index) count = search.property.directory.parent;
                search.argument.read_directory.entry = &entry;
                search.argument.read_directory.skip_name = 1;
                if (!func_0203e118(&search, 3)) {
                    do {
                        if (!entry.is_directory && entry.entry.file.id == file_id) {
                            directory = search.property.directory.position.directory;
                            break;
                        }
                    } while (!func_0203e118(&search, 3));
                }
                if (directory != 0x10000) break;
                ++index;
            } while (index < count);
        }
    }
    if (directory == 0x10000) {
        request->length = 0;
        return 1;
    }
    if (!request->length) {
        u32 current;
        u32 count = 0;
        if (archive->name <= 0xff) count += 1;
        else if (archive->name <= 0xff00) count += 2;
        else count += 3;
        count += 2;
        if (file_id != 0x10000) count += entry.name_length;
        current = directory;
        if (directory) {
            func_0203e034(&search, directory);
            do {
                func_0203e034(&search, search.property.directory.parent);
                search.argument.read_directory.entry = &entry;
                search.argument.read_directory.skip_name = 1;
                if (!func_0203e118(&search, 3)) {
                    do {
                        if (entry.is_directory && entry.entry.directory.directory == current) {
                            count += entry.name_length + 1;
                            break;
                        }
                    } while (!func_0203e118(&search, 3));
                }
                current = search.property.directory.position.directory;
            } while (current);
        }
        request->length = count + 1;
        request->directory = directory;
    }
    buffer = request->buffer;
    if (!buffer) return 0;
    length = request->length;
    if (request->capacity < length) return 1;
    {
        u32 offset = 0;
        u32 name_length;
        if (archive->name <= 0xff) name_length = 1;
        else if (archive->name <= 0xff00) name_length = 2;
        else name_length = 3;
        MI_CpuCopy8(&archive->name, buffer, name_length);
        offset += name_length;
        MI_CpuCopy8(data_02059d94, buffer + offset, 2);
    }
    func_0203e034(&search, directory);
    if (file_id != 0x10000) {
        u32 name_length;
        search.argument.read_directory.entry = &entry;
        search.argument.read_directory.skip_name = 0;
        if (!func_0203e118(&search, 3)) {
            do {
                if (!entry.is_directory && entry.entry.file.id == file_id) break;
            } while (!func_0203e118(&search, 3));
        }
        name_length = entry.name_length + 1;
        MI_CpuCopy8(entry.name, buffer + length - name_length, name_length);
        length -= name_length;
    } else {
        (buffer + length)[-1] = 0;
        --length;
    }
    if (directory) {
        do {
            func_0203e034(&search, search.property.directory.parent);
            search.argument.read_directory.entry = &entry;
            search.argument.read_directory.skip_name = 0;
            (buffer + length)[-1] = '/';
            --length;
            if (!func_0203e118(&search, 3)) {
                do {
                    if (entry.is_directory && entry.entry.directory.directory == directory) {
                        u32 name_length = entry.name_length;
                        MI_CpuCopy8(entry.name, buffer + length - name_length, name_length);
                        length -= name_length;
                        break;
                    }
                } while (!func_0203e118(&search, 3));
            }
            directory = search.property.directory.position.directory;
        } while (directory);
    }
    return 0;
}

int FSi_OpenFileFastCommand(FsFile *file)
{
    u32 entry[2];
    FsReadCursor cursor;
    FsArchive *archive = file->archive;
    u32 id = file->argument.file_id.id;
    if (id * 8 >= archive->fat_size) return 1;
    cursor.archive = archive;
    cursor.position = archive->fat + id * 8;
    func_0203e068(&cursor, entry, sizeof(entry));
    file->argument.direct.top = entry[0];
    file->argument.direct.bottom = entry[1];
    file->argument.direct.id = id;
    return func_0203e118(file, 7);
}

int FSi_OpenFileDirectCommand(FsFile *file)
{
    file->property.file.top = file->argument.direct.top;
    file->property.file.position = file->argument.direct.top;
    file->property.file.bottom = file->argument.direct.bottom;
    file->property.file.id = file->argument.direct.id;
    return 0;
}
