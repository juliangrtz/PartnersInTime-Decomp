#include <game/archive_io.h>

extern u32 data_02059c2c[];
extern void func_02029bb0(void *allocation);

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
    func_02029bb0(archive);
    return archive;
}

ArchiveCompressedIO *ArchiveCompressedIO_DestroyBase(ArchiveCompressedIO *archive)
{
    archive->base.vtable = data_02059c2c;
    ArchiveIO_DestroyBase(&archive->base);
    return archive;
}
