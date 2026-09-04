#include <game/battle_archive.h>
#include <game/battle_interface_assets.h>
#include <game/save_data.h>

enum BattleInterfaceConstant {
    SAVE_LANGUAGE_OFFSET = 0x515,
    BATTLE_INTERFACE_STATIC_DATA_OFFSET = 0x37608
};

extern void *gBattleSystem;
extern const void *data_ov002_020bebcc;
extern const void *data_ov002_020bebd4[];
extern const u8 data_ov002_020bf6d8[];
extern const u8 data_ov002_020bf6e8[];
extern const u8 data_ov002_020bf6f4[];

void *func_02029928(u32 heap_id, u32 size, const void *allocator, int use_default);
void func_020262f4(BattleInterfaceLayer *layer, void *asset_table,
                   void *asset, int unknown_3, int unknown_4, int unknown_5,
                   int unknown_6, int unknown_7, int unknown_8, int unknown_9,
                   int unknown_10, int unknown_11, int unknown_12,
                   int unknown_13);

void BattleInterface_InitLayersTask(BattleQueuedTask *task);
void BattleInterface_LoadSharedAssetTask(BattleQueuedTask *task);
void BattleInterface_LoadLocalizedAssetTask(BattleQueuedTask *task);
void BattleInterface_OpenArchiveTask(BattleQueuedTask *task);
void BattleInterface_LoadScreenAssetTask(BattleQueuedTask *task);
void BattleInterface_OpenScreenArchiveTask(BattleQueuedTask *task);

BattleQueuedTask *BattleInterface_RequestScreenLoad(int screen_index) {
    return BattleTaskQueue_Enqueue(BattleInterface_OpenScreenArchiveTask,
                                   (void *)screen_index);
}

void BattleInterface_OpenScreenArchiveTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    u32 descriptor_size;
    int screen_index = (int)task->argument;

    context->asset_read.archive_cursor =
        (u8 *)context->runtime.common_assets.header;
    ((BattleContext *)gBattleContext)->asset_read.descriptor =
        data_ov002_020bebd4[screen_index];
    descriptor_size = BattleArchive_GetDescriptorSize(
        gBattleSystem, ((BattleContext *)gBattleContext)->asset_read.descriptor);
    BattleArchive_Open(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        descriptor_size,
        ((BattleContext *)gBattleContext)->asset_read.descriptor);
    task->callback = BattleInterface_LoadScreenAssetTask;
}

void BattleInterface_LoadScreenAssetTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    const u8 *archive_cursor;
    void *destination;
    int screen_index = (int)task->argument;

    context->asset_read.entry_index = 0;
    archive_cursor = ((BattleContext *)gBattleContext)->asset_read.archive_cursor;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        *(u32 *)(archive_cursor + (*(u32 *)archive_cursor & ~3) - 4);
    destination = func_02029928(
        ((BattleContext *)gBattleContext)->runtime.heap_id,
        ((BattleContext *)gBattleContext)->asset_read.read_size,
        data_ov002_020bf6f4, 1);
    ((BattleContext *)gBattleContext)->asset_read.destination = destination;
    *(u32 *)((BattleContext *)gBattleContext)->asset_read.archive_cursor = 0;
    *(u32 *)(((BattleContext *)gBattleContext)->asset_read.archive_cursor + 4) =
        ((BattleContext *)gBattleContext)->asset_read.read_size;
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
    if (screen_index == 0) {
        ((BattleContext *)gBattleContext)->screen_assets[0] =
            ((BattleContext *)gBattleContext)->asset_read.destination;
    } else {
        ((BattleContext *)gBattleContext)->screen_assets[1] =
            ((BattleContext *)gBattleContext)->asset_read.destination;
    }
    task->callback = 0;
}

BattleQueuedTask *BattleInterface_RequestLoad(void) {
    return BattleTaskQueue_Enqueue(BattleInterface_OpenArchiveTask, 0);
}

void BattleInterface_OpenArchiveTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    u32 descriptor_size;

    context->asset_read.archive_cursor =
        (u8 *)context->runtime.common_assets.header;
    ((BattleContext *)gBattleContext)->asset_read.descriptor =
        data_ov002_020bebcc;
    descriptor_size = BattleArchive_GetDescriptorSize(
        gBattleSystem, ((BattleContext *)gBattleContext)->asset_read.descriptor);
    BattleArchive_Open(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        descriptor_size,
        ((BattleContext *)gBattleContext)->asset_read.descriptor);
    task->callback = BattleInterface_LoadLocalizedAssetTask;
}

void BattleInterface_LoadLocalizedAssetTask(BattleQueuedTask *task) {
    BattleArchiveReadRequest *request;
    s16 entry_index;

    if (*(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) == 0) {
        entry_index = 3;
    } else {
        entry_index = 8;
    }
    ((BattleContext *)gBattleContext)->asset_read.entry_index = entry_index;
    request = &((BattleContext *)gBattleContext)->asset_read;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        (BattleArchive_GetEntrySize(gBattleSystem, request->archive_cursor,
                                    request->entry_index) + 3) & ~3;
    ((BattleContext *)gBattleContext)->interface_assets[0] = func_02029928(
        ((BattleContext *)gBattleContext)->runtime.heap_id,
        ((BattleContext *)gBattleContext)->asset_read.read_size,
        data_ov002_020bf6e8, 1);
    ((BattleContext *)gBattleContext)->asset_read.destination =
        ((BattleContext *)gBattleContext)->interface_assets[0];
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
    task->callback = BattleInterface_LoadSharedAssetTask;
}

void BattleInterface_LoadSharedAssetTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    BattleArchiveReadRequest *request;

    context->asset_read.entry_index = 4;
    request = &((BattleContext *)gBattleContext)->asset_read;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        (BattleArchive_GetEntrySize(gBattleSystem, request->archive_cursor,
                                    request->entry_index) + 3) & ~3;
    ((BattleContext *)gBattleContext)->interface_assets[1] = func_02029928(
        ((BattleContext *)gBattleContext)->runtime.heap_id,
        ((BattleContext *)gBattleContext)->asset_read.read_size,
        data_ov002_020bf6e8, 1);
    ((BattleContext *)gBattleContext)->asset_read.destination =
        ((BattleContext *)gBattleContext)->interface_assets[1];
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
    task->callback = BattleInterface_InitLayersTask;
}

void BattleInterface_InitLayersTask(BattleQueuedTask *task) {
    BattleInterfaceLayer *layer;
    void *asset;

    layer = &((BattleContext *)gBattleContext)->interface_layer_0;
    layer->allocation_size = 0x4000;
    layer->vram_offset = 0x38000;
    layer->x = -42;
    layer->y = -8;
    layer->width = 144;
    layer->height = 72;
    func_020262f4(layer,
                   ((BattleContext *)gBattleContext)->interface_assets,
                   gBattleContext + BATTLE_INTERFACE_STATIC_DATA_OFFSET,
                   0, 0, 0, 1, 3, 1, 6, 31, 0, 255, 0);

    layer = &((BattleContext *)gBattleContext)->interface_layer_1;
    layer->allocation_size = 0x800;
    layer->vram_offset = 0x3C000;
    layer->x = -65;
    layer->y = -8;
    layer->width = 128;
    layer->height = 16;
    asset = func_02029928(
        ((BattleContext *)gBattleContext)->runtime.heap_id,
        layer->allocation_size, data_ov002_020bf6d8, 1);
    func_020262f4(layer,
                   ((BattleContext *)gBattleContext)->interface_assets,
                   asset, 0, 0, 0, 1, 3, 1, 6, 31, 0, 255, 0);

    layer = &((BattleContext *)gBattleContext)->interface_layer_2;
    layer->allocation_size = 0x400;
    layer->vram_offset = 0x3C800;
    layer->x = -64;
    layer->y = -8;
    layer->width = 128;
    layer->height = 16;
    asset = func_02029928(
        ((BattleContext *)gBattleContext)->runtime.heap_id,
        layer->allocation_size, data_ov002_020bf6d8, 1);
    func_020262f4(layer,
                   ((BattleContext *)gBattleContext)->interface_assets,
                   asset, 0, 0, 0, 1, 3, 1, 6, 15, 0, 255, 0);

    layer = &((BattleContext *)gBattleContext)->interface_layer_3;
    layer->allocation_size = 0x800;
    layer->vram_offset = 0x3CC00;
    layer->x = -52;
    layer->y = -48;
    layer->width = 128;
    layer->height = 32;
    asset = func_02029928(
        ((BattleContext *)gBattleContext)->runtime.heap_id,
        layer->allocation_size, data_ov002_020bf6d8, 1);
    func_020262f4(layer,
                   ((BattleContext *)gBattleContext)->interface_assets,
                   asset, 0, 0, 0, 1, 3, 1, 6, 15, 0, 255, 0);

    task->callback = 0;
}
