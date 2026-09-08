#include <game/field_archive.h>
#include <game/battle_archive.h>
#include <game/heap.h>

extern u32 data_ov000_020c0ed4[];
extern const char *data_ov000_020c0f30[];

FieldArchive *FieldArchive_Init(FieldArchive *archive, u32 priority, u32 unused, void *argument, int dma)
{
    unsigned i;
    ArchiveCompressedIO_InitComplete(&archive->base, priority, unused, argument, dma);
    archive->base.base.vtable = data_ov000_020c0ed4;
    archive->load_phase = 0;
    for (i = 0; i < 7; ++i) {
        archive->descriptor_sizes[i] = BattleArchive_GetDescriptorSize(archive, data_ov000_020c0f30[i]);
        archive->descriptors[i] = GameHeap_NewArray(4 * (archive->descriptor_sizes[i] >> 2), 0, 0, 1);
        BattleArchive_Open(archive, archive->descriptors[i], archive->descriptor_sizes[i],
                           data_ov000_020c0f30[i]);
        ArchiveIO_WaitOpen(&archive->base.base, data_ov000_020c0f30[i]);
    }
    return archive;
}

FieldArchive *FieldArchive_Destroy(FieldArchive *archive)
{
    unsigned i;
    int j;
    archive->base.base.vtable = data_ov000_020c0ed4;
    for (i = 0; i < 7; ++i)
        GameHeap_DeleteArray(archive->descriptors[i]);
    for (j = 0; j < 3; ++j)
        GameHeap_DeleteArray(archive->resource_data[j]);
    ArchiveCompressedIO_DestroyBase(&archive->base);
    return archive;
}

FieldArchive *FieldArchive_Delete(FieldArchive *archive)
{
    unsigned i;
    int j;
    archive->base.base.vtable = data_ov000_020c0ed4;
    for (i = 0; i < 7; ++i)
        GameHeap_DeleteArray(archive->descriptors[i]);
    for (j = 0; j < 3; ++j)
        GameHeap_DeleteArray(archive->resource_data[j]);
    ArchiveCompressedIO_DestroyBase(&archive->base);
    GameHeap_Delete(archive);
    return archive;
}
