#include <game/battle_archive.h>
#include <game/battle_common_assets.h>
#include <game/save_data.h>

enum BattleCommonAssetConstant {
    SAVE_LANGUAGE_OFFSET = 0x515,
    BATTLE_COMMON_ASSET_HEADER_READ_SIZE = 8
};

extern void *gBattleSystem;
extern void *data_02059f64;
extern const void *data_ov002_020bebc8;

void BattleCommonAssets_OpenArchiveTask(BattleQueuedTask *task);
void BattleCommonAssets_ReadHeaderTask(BattleQueuedTask *task);
void BattleCommonAssets_ReadOffsetsTask(BattleQueuedTask *task);
void BattleCommonAssets_LoadEntriesTask(BattleQueuedTask *task);
void BattleCommonAssets_LoadEntry(int entry_index, int asset_index);

BattleQueuedTask *BattleCommonAssets_RequestLoad(int archive_index) {
    BattleContext *context = (BattleContext *)gBattleContext;

    if (context->runtime.common_assets.source == 0) {
        return 0;
    }
    context->runtime.flags.raw |= BATTLE_RUNTIME_FLAG_COMMON_ASSET_LOAD_PENDING;
    return BattleTaskQueue_Enqueue(BattleCommonAssets_OpenArchiveTask,
                                   (void *)archive_index);
}

void BattleCommonAssets_OpenArchiveTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    u32 descriptor_size;

    context->asset_read.archive_cursor = context->runtime.common_assets.source;
    ((BattleContext *)gBattleContext)->asset_read.descriptor =
        data_ov002_020bebc8;
    descriptor_size = BattleArchive_GetDescriptorSize(
        gBattleSystem, ((BattleContext *)gBattleContext)->asset_read.descriptor);
    BattleArchive_Open(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        descriptor_size,
        ((BattleContext *)gBattleContext)->asset_read.descriptor);
    task->callback = BattleCommonAssets_ReadHeaderTask;
}

void BattleCommonAssets_ReadHeaderTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;

    context->asset_read.entry_index = (u16)(u32)task->argument;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        BATTLE_COMMON_ASSET_HEADER_READ_SIZE;
    ((BattleContext *)gBattleContext)->asset_read.destination =
        (u8 *)((BattleContext *)gBattleContext)->runtime.common_assets.header;
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
    task->callback = BattleCommonAssets_ReadOffsetsTask;
}

void BattleCommonAssets_ReadOffsetsTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;

    context->asset_read.entry_index = (u16)(u32)task->argument;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        ((BattleContext *)gBattleContext)->runtime.common_assets.header[0];
    ((BattleContext *)gBattleContext)->asset_read.destination =
        (u8 *)((BattleContext *)gBattleContext)->runtime.common_assets.header;
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
    task->callback = BattleCommonAssets_LoadEntriesTask;
}

void BattleCommonAssets_LoadEntriesTask(BattleQueuedTask *task) {
    int offset_count;
    u32 archive_offset;
    int asset_index;

    if (((BattleContext *)gBattleContext)->asset_read.archive_cursor !=
        (u8 *)((BattleContext *)gBattleContext)
            ->runtime.common_assets.header) {
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor =
            (u8 *)((BattleContext *)gBattleContext)
                ->runtime.common_assets.header;
        offset_count = ((BattleContext *)gBattleContext)
                           ->runtime.common_assets.header[0] >> 2;
        archive_offset = *(u32 *)(((BattleContext *)gBattleContext)
                                      ->runtime.common_assets.source +
                                  (u32)task->argument * sizeof(u32));
        for (asset_index = 0; asset_index < offset_count; ++asset_index) {
            ((BattleContext *)gBattleContext)
                ->runtime.common_assets.header[asset_index] += archive_offset;
        }
        ((BattleContext *)gBattleContext)->asset_read.destination =
            (u8 *)((BattleContext *)gBattleContext)
                ->runtime.common_assets.source;
        task->timer = 0;
    }

    switch (task->timer) {
    case 0:
        ((BattleContext *)gBattleContext)->common_asset_pointers[0] =
            data_02059f64;
        break;
    case 1:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 5, task->timer);
        break;
    case 2:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 5 + 1, task->timer);
        break;
    case 3:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 5 + 2, task->timer);
        break;
    case 4:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 5 + 3, task->timer);
        break;
    case 5:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 5 + 4, task->timer);
        break;
    case 6:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 6 + 30,
            task->timer);
        break;
    case 7:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 6 + 31,
            task->timer);
        break;
    case 8:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 6 + 32,
            task->timer);
        break;
    case 9:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 6 + 33,
            task->timer);
        break;
    case 10:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 6 + 34,
            task->timer);
        break;
    case 11:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 6 + 35,
            task->timer);
        break;
    case 12:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 2 + 66,
            task->timer);
        break;
    case 13:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) * 2 + 67,
            task->timer);
        break;
    case 14:
        BattleCommonAssets_LoadEntry(78, task->timer);
        break;
    case 15:
        BattleCommonAssets_LoadEntry(81, task->timer);
        break;
    case 16:
        BattleCommonAssets_LoadEntry(79, task->timer);
        break;
    case 17:
        BattleCommonAssets_LoadEntry(82, task->timer);
        break;
    case 18:
        BattleCommonAssets_LoadEntry(80, task->timer);
        break;
    case 19:
        BattleCommonAssets_LoadEntry(83, task->timer);
        break;
    case 20:
        BattleCommonAssets_LoadEntry(
            *(u8 *)(gSaveData + SAVE_LANGUAGE_OFFSET) + 84, task->timer);
        break;
    default:
        ((BattleContext *)gBattleContext)->common_asset_end =
            ((BattleContext *)gBattleContext)->common_asset_pointers[20];
        task->callback = 0;
        ((BattleContext *)gBattleContext)->runtime.flags.raw &=
            ~BATTLE_RUNTIME_FLAG_COMMON_ASSET_LOAD_PENDING;
        return;
    }
    ++task->timer;
}

void BattleCommonAssets_LoadEntry(int entry_index, int asset_index) {
    BattleContext *context = (BattleContext *)gBattleContext;
    BattleArchiveReadRequest *request;

    context->asset_read.entry_index = entry_index;
    ((BattleContext *)gBattleContext)->asset_read.destination +=
        ((BattleContext *)gBattleContext)->asset_read.read_size;
    request = &((BattleContext *)gBattleContext)->asset_read;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        (BattleArchive_GetEntrySize(gBattleSystem, request->archive_cursor,
                                    request->entry_index) + 3) & ~3;
    if (((BattleContext *)gBattleContext)->asset_read.read_size == 0) {
        ((BattleContext *)gBattleContext)->common_asset_pointers[asset_index] = 0;
        return;
    }
    ((BattleContext *)gBattleContext)->common_asset_pointers[asset_index] =
        ((BattleContext *)gBattleContext)->asset_read.destination;
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
}
