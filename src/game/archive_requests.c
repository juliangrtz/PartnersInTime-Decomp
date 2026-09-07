#include <game/archive_io.h>
#include <game/battle_archive.h>

u32 BattleArchive_GetDescriptorSize(void *system, const void *descriptor)
{
    ArchiveIO *archive = system;
    u32 size;
    if (!ArchiveIO_OpenFile(archive, descriptor)) return 0;
    FS_ReadFile(&archive->file, &size, 4);
    return size;
}

int BattleArchive_Open(void *system, const u8 *source, u32 size, const void *descriptor)
{
    ArchiveIO *archive = system;
    ArchiveOpenRequest *request = &archive->opens[archive->open_tail];
    if (archive->open_tail == 7) archive->open_tail = 0;
    else archive->open_tail++;
    if (archive->open_head == archive->open_tail) {
        if (!archive->open_tail) archive->open_tail = 7;
        else archive->open_tail--;
        return 0;
    }
    request->destination = (void *)source;
    request->size = size;
    request->path = descriptor;
    request->state = 1;
    return ArchiveIO_ProcessOpen(archive);
}

void ArchiveIO_WaitOpen(ArchiveIO *archive, const char *path)
{
    u16 end;
    u16 index = archive->open_head;
    end = archive->open_tail;
    while (index != end) {
        if (archive->opens[index].path == path) {
            ArchiveOpenRequest *request = &archive->opens[index];
            while (request->state) ArchiveIO_ProcessOpen(archive);
            return;
        }
        if (index == 7) index = 0;
        else index++;
    }
}

int ArchiveIO_GetOpenState(ArchiveIO *archive, const char *path)
{
    u16 index = archive->open_head;
    u16 end = archive->open_tail;
    while (index != end) {
        if (archive->opens[index].path == path) return archive->opens[index].state;
        if (index == 7) index = 0;
        else index++;
    }
    return 0;
}

u32 BattleArchive_GetEntrySize(void *system, const u8 *archive_cursor, u16 entry_index)
{
    const u32 *offsets = (const u32 *)archive_cursor;
    return (&offsets[entry_index])[1] - offsets[entry_index];
}

void ArchiveIO_WaitRead(ArchiveIO *archive, ArchiveReadRequest *request)
{
    ArchiveReadRequest *cursor;
    for (cursor = archive->first; cursor; cursor = cursor->next) {
        if (cursor == request) break;
    }
    if (cursor) {
        while (request->state) ArchiveIO_ProcessRead(archive);
    }
}
