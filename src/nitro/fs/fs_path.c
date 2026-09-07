#include <nitro/fs.h>

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
