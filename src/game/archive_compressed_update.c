#include <game/archive_io.h>

extern s16 data_02060b2c[];

static inline int ArchiveIO_GetVCount(void)
{
    return *(volatile u16 *)0x04000006;
}

void ArchiveIO_UpdateCompressed(ArchiveIO *archive)
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
            ArchiveIO_ProcessCompressedRead(archive);
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
