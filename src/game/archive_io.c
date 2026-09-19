#include <game/archive_io.h>
#include <game/task.h>

extern u32 data_02059bdc[];
extern s16 data_02060b2c[];
extern void GameHeap_Delete(void *allocation);
extern int FS_SetDefaultDMA(int dma);

ArchiveIO *ArchiveIO_InitComplete(ArchiveIO *archive, u32 priority, u32 unused, void *argument, int dma)
{
    GameTask_Init((GameTask *)archive, priority, unused, argument);
    archive->vtable = data_02059bdc;
    archive->last = 0;
    archive->first = archive->last;
    archive->scanline = 168;
    archive->budget_frames = 5;
    archive->interval_frames = 2;
    archive->current_frame = 0;
    archive->open_tail = archive->current_frame;
    archive->open_head = archive->open_tail;
    archive->overlay_state = 0;
    if (dma != -32897) FS_SetDefaultDMA(dma);
    FS_InitFile(&archive->file);
    return archive;
}

ArchiveIO *ArchiveIO_InitBase(ArchiveIO *archive, u32 priority, u32 unused, void *argument, int dma)
{
    GameTask_Init((GameTask *)archive, priority, unused, argument);
    archive->vtable = data_02059bdc;
    archive->last = 0;
    archive->first = archive->last;
    archive->scanline = 168;
    archive->budget_frames = 5;
    archive->interval_frames = 2;
    archive->current_frame = 0;
    archive->open_tail = archive->current_frame;
    archive->open_head = archive->open_tail;
    archive->overlay_state = 0;
    if (dma != -32897) FS_SetDefaultDMA(dma);
    FS_InitFile(&archive->file);
    return archive;
}

ArchiveIO *ArchiveIO_DestroyComplete(ArchiveIO *archive)
{
    archive->vtable = data_02059bdc;
    if (FSi_TestFileFlag(&archive->file, 16)) FS_CloseFile(&archive->file);
    GameTask_DestroyBase((GameTask *)archive);
    return archive;
}

ArchiveIO *ArchiveIO_Delete(ArchiveIO *archive)
{
    archive->vtable = data_02059bdc;
    if (FSi_TestFileFlag(&archive->file, 16)) FS_CloseFile(&archive->file);
    GameTask_DestroyBase((GameTask *)archive);
    GameHeap_Delete(archive);
    return archive;
}

ArchiveIO *ArchiveIO_DestroyBase(ArchiveIO *archive)
{
    archive->vtable = data_02059bdc;
    if (FSi_TestFileFlag(&archive->file, 16)) FS_CloseFile(&archive->file);
    GameTask_DestroyBase((GameTask *)archive);
    return archive;
}

static inline int ArchiveIO_GetVCount(void)
{
    return *(volatile u16 *)0x04000006;
}

void ArchiveIO_Update(ArchiveIO *archive)
{
    if (!data_02060b2c[15] &&
        (ArchiveIO_GetVCount() < archive->scanline || ArchiveIO_GetVCount() >= 192)) {
        archive->current_frame = 0;
    } else if (archive->current_frame >= archive->interval_frames) {
        archive->current_frame = archive->budget_frames;
    } else {
        archive->current_frame++;
        if (archive->current_frame != archive->budget_frames + 1) return;
    }
    do {
        if (archive->first) {
            ArchiveIO_ProcessRead(archive);
        } else if (archive->open_head != archive->open_tail) {
            ArchiveIO_ProcessOpen(archive);
        } else {
            if (archive->overlay_state) {
                ArchiveIO_ProcessOverlay(archive);
            } else {
                archive->current_frame = 0;
                return;
            }
        }
    } while (!data_02060b2c[15] &&
        (ArchiveIO_GetVCount() < archive->scanline || ArchiveIO_GetVCount() >= 192));
}

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

extern void *data_02060d90;
extern u16 data_02060d8c;
extern FsOverlayInfo data_02060d94;
extern FsFile data_02060dc0;
extern void FS_Init(u32 dma);
extern u32 FS_TryLoadTable(void *buffer, u32 size);
extern int FS_LoadOverlay(int processor, u32 id);
extern int FS_UnloadOverlay(int processor, u32 id);
extern int FS_LoadOverlayImageAsync(FsOverlayInfo *overlay, FsFile *file);

static inline int ArchiveIO_FileTablesFit(void *buffer, u32 size)
{
    if (FS_TryLoadTable(buffer, size) <= size) return 1;
    return 0;
}

int ArchiveIO_LoadOverlay(u32 id, int processor)
{
    return FS_LoadOverlay(processor, id);
}

int ArchiveIO_BeginGlobalOverlay(u32 id, u32 unused, int processor)
{
    if (!FS_LoadOverlayInfo(&data_02060d94, processor, id)) return 0;
    FS_ClearOverlayImage(&data_02060d94);
    if (!data_02060d8c) {
        FS_InitFile(&data_02060dc0);
        data_02060d8c = 1;
    }
    if (!FS_LoadOverlayImageAsync(&data_02060d94, &data_02060dc0)) return 0;
    return 1;
}

int ArchiveIO_FinishGlobalOverlay(void)
{
    if (FS_IsBusy(&data_02060dc0)) return 2;
    FS_CloseFile(&data_02060dc0);
    FS_StartOverlay(&data_02060d94);
    return 0;
}

int ArchiveIO_BeginOverlay(ArchiveIO *archive, u32 id, int processor)
{
    FsFileId file;
    FsOverlayInfo *overlay = &archive->overlay;
    if (!FS_LoadOverlayInfo(overlay, processor, id)) return 0;
    FS_ClearOverlayImage(overlay);
    if (FSi_TestFileFlag(&archive->file, 16)) FS_CloseFile(&archive->file);
    file = FS_GetOverlayFileID(overlay);
    if (!FS_OpenFileFast(&archive->file, file)) return 0;
    FS_ClearOverlayImage(overlay);
    archive->overlay_cursor = overlay->address;
    archive->overlay_remaining = overlay->ram_size;
    archive->overlay_chunk = archive->overlay_remaining > 512 ? 512 : archive->overlay_remaining;
    archive->overlay_state = 1;
    ArchiveIO_ProcessOverlay(archive);
    return 1;
}

int ArchiveIO_FinishOverlay(ArchiveIO *archive)
{
    if (archive->overlay_state) return 2;
    FS_CloseFile(&archive->file);
    FS_StartOverlay(&archive->overlay);
    return 0;
}

int ArchiveIO_UnloadOverlay(u32 id, int processor)
{
    return FS_UnloadOverlay(processor, id);
}

u32 ArchiveIO_InitFilesystem(u32 dma, void *buffer, u32 size)
{
    FS_Init(dma);
    if (buffer && ArchiveIO_FileTablesFit(buffer, size)) data_02060d90 = buffer;
    return FS_TryLoadTable(0, 0);
}

u32 ArchiveIO_GetFileTableSize(void)
{
    return FS_TryLoadTable(0, 0);
}

int ArchiveIO_LoadFileTables(void *buffer, u32 size)
{
    int fits;
    if (data_02060d90) return 0;
    fits = FS_TryLoadTable(buffer, size) <= size;
    if (!fits) return 0;
    data_02060d90 = buffer;
    return 1;
}

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
