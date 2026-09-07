#include <game/archive_io.h>
#include <game/battle_archive.h>

extern void func_0202bf10(u8 **source, u8 **destination);
extern void func_0202bfbc(u8 **source, u8 **destination);

u32 ArchiveIO_PrepareCompressedRead(ArchiveIO *archive, ArchiveCompressedRequest *request, const u32 *offsets, const char *path)
{
    u8 header[8];
    u32 blocks;
    if (offsets) request->read.offsets = offsets;
    request->read.cursor = header;
    request->read.path = path;
    request->read.chunk_size = 8;
    request->read.offset = 0;
    ArchiveIO_ReadImmediate(archive, &request->read);
    request->read.cursor = header;
    ArchiveCodec_ReadHeader(&request->read.cursor, &blocks, &request->output_size);
    request->read.offset = request->read.cursor - header;
    request->compressed_state = 1;
    request->blocks = blocks;
    return request->output_size;
}

int ArchiveIO_QueueCompressedRead(ArchiveIO *archive, ArchiveCompressedRequest *request, void *destination)
{
    request->destination = destination;
    request->write_cursor = request->destination;
    request->read.state = 1;
    request->compressed_state = 2;
    request->read.next = 0;
    request->read.cursor = (u8 *)&request->read.chunk_size;
    request->read.chunk_size = 2;
    ArchiveIO_ReadImmediate(archive, &request->read);
    request->read.offset += 2;
    request->read.cursor = ((ArchiveCompressedIO *)archive)->buffer;
    if (archive->last) {
        archive->last->next = &request->read;
        archive->last = &request->read;
    } else {
        archive->last = &request->read;
        archive->first = archive->last;
        ArchiveIO_SelectDecoder(archive, request);
    }
    return ArchiveIO_ProcessCompressedRead(archive);
}

void ArchiveIO_WaitCompressedRead(ArchiveIO *archive, ArchiveReadRequest *request)
{
    ArchiveReadRequest *cursor;
    for (cursor = archive->first; cursor; cursor = cursor->next) {
        if (cursor == request) break;
    }
    if (cursor) {
        while (request->state) ArchiveIO_ProcessCompressedRead(archive);
    }
}

int BattleArchive_ReadAsync(void *system, void *request_memory, int offset, const u8 *archive_cursor, const void *descriptor, u16 flags)
{
    ArchiveIO *archive = system;
    ArchiveCompressedRequest *request = request_memory;
    u32 size;
    int use_entry_size;
    if (archive_cursor) request->read.offsets = (const u32 *)archive_cursor;
    request->read.state = flags | 1;
    request->compressed_state = 0;
    request->read.offset = offset;
    request->read.path = descriptor;
    request->read.cursor = request->read.destination;
    request->read.next = 0;
    size = BattleArchive_GetEntrySize(archive, (const u8 *)request->read.offsets, request->read.entry) - offset;
    use_entry_size = 1;
    if (request->read.limit && request->read.limit <= size) use_entry_size = 0;
    if (!use_entry_size) size = request->read.limit;
    request->read.remaining = size;
    request->read.chunk_size = flags && request->read.remaining > 512 ? 512 : request->read.remaining;
    if (archive->last) {
        archive->last->next = &request->read;
        archive->last = &request->read;
    } else {
        archive->last = &request->read;
        archive->first = archive->last;
    }
    return ArchiveIO_ProcessCompressedRead(archive);
}

int ArchiveIO_ReadImmediate(ArchiveIO *archive, ArchiveReadRequest *request)
{
    while (FS_IsBusy(&archive->file)) {}
    if (request->path) {
        int result = ArchiveIO_OpenFile(archive, request->path);
        if (!result) return result;
    }
    FS_SeekFile(&archive->file, request->offset + request->offsets[request->entry], 0);
    return FS_ReadFile(&archive->file, request->cursor, request->chunk_size);
}

int ArchiveIO_ProcessCompressedRead(ArchiveIO *archive)
{
    ArchiveCompressedRequest *request;
    if (FS_IsBusy(&archive->file)) return 0;
    request = (ArchiveCompressedRequest *)archive->first;
    if ((request->read.state & 3) == 2) {
        if (ArchiveIO_FileSucceeded(&archive->file)) {
            if (request->compressed_state) {
                request->read.cursor = ((ArchiveCompressedIO *)archive)->buffer;
                request->decode(&request->read.cursor, &request->write_cursor);
                if (--request->blocks == 0) {
                    request->read.state = 0;
                    request->compressed_state = 0;
                    archive->first = request->read.next;
                    request->read.next = 0;
                    if (!archive->first) {
                        archive->last = 0;
                        return 1;
                    }
                    if (((ArchiveCompressedRequest *)archive->first)->compressed_state) {
                        ArchiveIO_SelectDecoder(archive, (ArchiveCompressedRequest *)archive->first);
                        archive->first->cursor = ((ArchiveCompressedIO *)archive)->buffer;
                    }
                    request = (ArchiveCompressedRequest *)archive->first;
                    request->read.state = (request->read.state & 0x8000) | 2;
                } else {
                    request->read.offset += request->read.chunk_size;
                    request->read.cursor = (u8 *)&request->read.chunk_size;
                    request->read.chunk_size = 2;
                    ArchiveIO_ReadImmediate(archive, &request->read);
                    request->read.offset += 2;
                    request->read.cursor = ((ArchiveCompressedIO *)archive)->buffer;
                }
            } else if ((request->read.state & 0x8000) && request->read.remaining > 512) {
                request->read.cursor += 512;
                request->read.offset += 512;
                request->read.remaining -= 512;
                if (request->read.remaining < 512) request->read.chunk_size = request->read.remaining;
            } else {
                request->read.state = 0;
                archive->first = request->read.next;
                request->read.next = 0;
                if (!archive->first) {
                    archive->last = 0;
                    return 1;
                }
                if (((ArchiveCompressedRequest *)archive->first)->compressed_state) {
                    ArchiveIO_SelectDecoder(archive, (ArchiveCompressedRequest *)archive->first);
                }
                request = (ArchiveCompressedRequest *)archive->first;
                request->read.state = (request->read.state & 0x8000) | 2;
            }
        }
    } else {
        request->read.state = (request->read.state & 0x8000) | 2;
    }
    if (request->read.path && !ArchiveIO_OpenFile(archive, request->read.path)) return 0;
    FS_SeekFile(&archive->file, request->read.offset + request->read.offsets[request->read.entry], 0);
    FS_ReadFile(&archive->file, request->read.cursor, request->read.chunk_size);
    if (request->compressed_state == 2) request->compressed_state = 3;
    return 1;
}

