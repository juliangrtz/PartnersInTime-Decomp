#include <game/archive_io.h>

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
