#include <game/archive_io.h>
#include <game/task.h>

extern u32 data_02059bdc[];
extern s16 data_02060b2c[];
extern void func_02029bb0(void *allocation);
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
    func_02029bb0(archive);
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
