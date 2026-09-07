#include <game/archive_io.h>

extern u32 data_02059c2c[];
extern void GameHeap_Delete(void *allocation);

ArchiveCompressedIO *ArchiveCompressedIO_InitComplete(ArchiveCompressedIO *archive, u32 priority, u32 unused, void *argument, int dma)
{
    ArchiveIO_InitComplete(&archive->base, priority, unused, argument, dma);
    archive->base.vtable = data_02059c2c;
    return archive;
}

ArchiveCompressedIO *ArchiveCompressedIO_InitBase(ArchiveCompressedIO *archive, u32 priority, u32 unused, void *argument, int dma)
{
    ArchiveIO_InitComplete(&archive->base, priority, unused, argument, dma);
    archive->base.vtable = data_02059c2c;
    return archive;
}

ArchiveCompressedIO *ArchiveCompressedIO_DestroyComplete(ArchiveCompressedIO *archive)
{
    archive->base.vtable = data_02059c2c;
    ArchiveIO_DestroyBase(&archive->base);
    return archive;
}

ArchiveCompressedIO *ArchiveCompressedIO_Delete(ArchiveCompressedIO *archive)
{
    archive->base.vtable = data_02059c2c;
    ArchiveIO_DestroyBase(&archive->base);
    GameHeap_Delete(archive);
    return archive;
}

ArchiveCompressedIO *ArchiveCompressedIO_DestroyBase(ArchiveCompressedIO *archive)
{
    archive->base.vtable = data_02059c2c;
    ArchiveIO_DestroyBase(&archive->base);
    return archive;
}
