#include <game/archive_io.h>
#include <game/battle_archive.h>

int ArchiveIO_OpenFile(ArchiveIO *archive, const char *path)
{
    if (FSi_TestFileFlag(&archive->file, 16)) FS_CloseFile(&archive->file);
    return FS_OpenFile(&archive->file, path);
}

int ArchiveIO_ProcessOpen(ArchiveIO *archive)
{
    ArchiveOpenRequest *request;
    if (FS_IsBusy(&archive->file)) return 0;
    request = &archive->opens[archive->open_head];
    if (request->state == 2 && ArchiveIO_FileSucceeded(&archive->file)) {
        if (archive->open_head == 7) archive->open_head = 0;
        else archive->open_head++;
        request->state = 0;
        if (archive->open_head == archive->open_tail) return 1;
        request = &archive->opens[archive->open_head];
    }
    if (request->path) {
        if (!ArchiveIO_OpenFile(archive, request->path)) return 0;
    } else {
        FS_SeekFile(&archive->file, 0, 0);
    }
    FS_ReadFile(&archive->file, request->destination, request->size);
    request->state = 2;
    return 1;
}

int ArchiveIO_QueueRead(ArchiveIO *archive, ArchiveReadRequest *request, u32 offset, const u32 *offsets, const char *path, u16 flags)
{
    u32 size;
    int use_entry_size;
    if (offsets) request->offsets = offsets;
    request->state = flags | 1;
    request->offset = offset;
    request->path = path;
    request->cursor = request->destination;
    request->next = 0;
    size = BattleArchive_GetEntrySize(archive, (const u8 *)request->offsets, request->entry) - offset;
    use_entry_size = 1;
    if (request->limit && request->limit <= size) use_entry_size = 0;
    if (!use_entry_size) size = request->limit;
    request->remaining = size;
    request->chunk_size = flags && request->remaining > 512 ? 512 : request->remaining;
    if (archive->last) {
        archive->last->next = request;
        archive->last = request;
    } else {
        archive->last = request;
        archive->first = archive->last;
    }
    return ArchiveIO_ProcessRead(archive);
}

int ArchiveIO_ProcessRead(ArchiveIO *archive)
{
    ArchiveReadRequest *request;
    if (FS_IsBusy(&archive->file)) return 0;
    request = archive->first;
    if ((request->state & 3) == 2) {
        if (ArchiveIO_FileSucceeded(&archive->file)) {
            if ((request->state & 0x8000) && request->remaining > 512) {
                request->cursor += 512;
                request->offset += 512;
                request->remaining -= 512;
                if (request->remaining < 512) request->chunk_size = request->remaining;
            } else {
                request->state = 0;
                archive->first = request->next;
                request->next = 0;
                request = archive->first;
                if (!request) {
                    archive->last = 0;
                    return 1;
                }
                request->state = (request->state & 0x8000) | 2;
            }
        }
    } else {
        request->state = (request->state & 0x8000) | 2;
    }
    if (request->path && !ArchiveIO_OpenFile(archive, request->path)) return 0;
    FS_SeekFile(&archive->file, request->offset + request->offsets[request->entry], 0);
    FS_ReadFile(&archive->file, request->cursor, request->chunk_size);
    return 1;
}

int ArchiveIO_ProcessOverlay(ArchiveIO *archive)
{
    if (FS_IsBusy(&archive->file)) return 0;
    if ((archive->overlay_state & 3) == 2) {
        if (ArchiveIO_FileSucceeded(&archive->file)) {
            if (archive->overlay_remaining <= 512) {
                archive->overlay_state = 0;
                return 1;
            }
            archive->overlay_cursor += 512;
            archive->overlay_remaining -= 512;
            if (archive->overlay_remaining < 512) archive->overlay_chunk = archive->overlay_remaining;
        }
    } else {
        archive->overlay_state = 2;
    }
    FS_ReadFile(&archive->file, archive->overlay_cursor, archive->overlay_chunk);
    return 1;
}
