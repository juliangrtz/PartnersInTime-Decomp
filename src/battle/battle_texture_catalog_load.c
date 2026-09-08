#include <game/battle_archive.h>
#include <game/heap.h>
#include <game/save_data.h>

extern void *gBattleSystem;
typedef struct BattleAllocationNames {
    void *values[3];
} BattleAllocationNames;
typedef char BattleAllocationNamesSize[sizeof(BattleAllocationNames) == 12 ? 1 : -1];
extern BattleAllocationNames data_ov002_020bec00, data_ov002_020bec3c;
extern const void *data_ov002_020bec48[3];
static inline BattleContext *Context(void)
{
    return (BattleContext *)gBattleContext;
}

void BattleTextureCatalog_OpenTask(BattleQueuedTask *task)
{
    void **catalog;
    void **source;
    const void *descriptor;
    int index;
    u32 size;
    BattleAllocationNames names = data_ov002_020bec3c;
    switch ((u32)task->argument) {
    case 0xc0000000:
        source = &Context()->runtime.object_texture_sources[0];
        catalog = (void **)&Context()->runtime.object_texture_catalogs[0];
        descriptor = data_ov002_020bec48[0];
        index = 0;
        break;
    case 0xc1000000:
        source = &Context()->runtime.object_texture_sources[1];
        catalog = (void **)&Context()->runtime.object_texture_catalogs[1];
        descriptor = data_ov002_020bec48[1];
        index = 1;
        break;
    case 0xc2000000:
        source = &Context()->runtime.object_texture_sources[2];
        catalog = (void **)&Context()->runtime.object_texture_catalogs[2];
        descriptor = data_ov002_020bec48[2];
        index = 2;
        break;
    }
    if (*catalog) {
        task->callback = 0;
        return;
    }
    size = (BattleArchive_GetDescriptorSize(gBattleSystem, descriptor) + 3) & ~3;
    *source = GameHeap_Allocate(Context()->runtime.heap_id, size, names.values[index], 1);
    Context()->asset_read.archive_cursor = *source;
    Context()->asset_read.descriptor = descriptor;
    BattleArchive_Open(gBattleSystem, *source, size, descriptor);
    task->callback = BattleTextureCatalog_ReadTask;
}

void BattleTextureCatalog_ReadTask(BattleQueuedTask *task)
{
    void **catalog;
    int index;
    u32 size;
    BattleAllocationNames names;
    Context()->asset_read.entry_index = 0;
    names = data_ov002_020bec00;
    switch ((u32)task->argument) {
    case 0xc0000000:
        index = 0;
        catalog = (void **)&Context()->runtime.object_texture_catalogs[0];
        break;
    case 0xc1000000:
        index = 1;
        catalog = (void **)&Context()->runtime.object_texture_catalogs[1];
        break;
    case 0xc2000000:
        index = 2;
        catalog = (void **)&Context()->runtime.object_texture_catalogs[2];
        break;
    }
    {
        BattleArchiveReadRequest *request = &Context()->asset_read;
        size =
            (BattleArchive_GetEntrySize(gBattleSystem, request->archive_cursor, request->entry_index) + 3) &
            ~3;
    }
    *catalog = GameHeap_Allocate(Context()->runtime.heap_id, size, names.values[index], 1);
    Context()->asset_read.destination = *catalog;
    Context()->asset_read.read_size = size;
    BattleArchive_ReadAsync(gBattleSystem, &Context()->asset_read, 0, Context()->asset_read.archive_cursor,
                            Context()->asset_read.descriptor, 0);
    task->callback = 0;
}
