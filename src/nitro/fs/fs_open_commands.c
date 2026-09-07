#include <nitro/fs.h>

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
