#include <game/battle_archive.h>
#include <game/battle_context.h>
#include <game/battle_enemy_data.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern void *gBattleSystem;
extern const void *gBattlePrimaryResourcePaths[];

void BattleEnemyData_FixupObjectPointers(BattleQueuedTask *task);
void BattleEnemyData_LoadObjectData(BattleQueuedTask *task);
void BattleEnemyData_LoadStatRecord(BattleQueuedTask *task);

BattleQueuedTask *BattleEnemyData_RequestLoad(BattleEnemyDataRequest *request,
                                              u16 stat_index) {
    request->stats = 0;
    request->stat_index = stat_index;
    return BattleTaskQueue_Enqueue(BattleEnemyData_LoadStatRecord, request);
}

void BattleEnemyData_LoadStatRecord(BattleQueuedTask *task) {
    BattleEnemyDataRequest *request =
        (BattleEnemyDataRequest *)task->argument;
    ((BattleContext *)gBattleContext)->asset_read.destination =
        request->payload.bytes;
    ((BattleContext *)gBattleContext)->asset_read.read_size =
        sizeof(BattleEnemyStatRecord);
    ((BattleContext *)gBattleContext)->asset_read.entry_index = 0;
    ((BattleContext *)gBattleContext)->asset_read.descriptor =
        gBattlePrimaryResourcePaths[0];
    ((BattleContext *)gBattleContext)->asset_read.archive_cursor =
        (const u8 *)((BattleContext *)gBattleContext)
            ->runtime.common_assets.header;
    ((u32 *)((BattleContext *)gBattleContext)
         ->asset_read.archive_cursor)[0] =
        request->stat_index * sizeof(BattleEnemyStatRecord);
    ((u32 *)((BattleContext *)gBattleContext)
         ->asset_read.archive_cursor)[1] =
        (request->stat_index + 1) * sizeof(BattleEnemyStatRecord);
    BattleArchive_ReadAsync(
        gBattleSystem, &((BattleContext *)gBattleContext)->asset_read, 0,
        ((BattleContext *)gBattleContext)->asset_read.archive_cursor,
        ((BattleContext *)gBattleContext)->asset_read.descriptor, 0);
    task->callback = BattleEnemyData_LoadObjectData;
}

void BattleEnemyData_LoadObjectData(BattleQueuedTask *task) {
    BattleEnemyDataRequest *request =
        (BattleEnemyDataRequest *)task->argument;
    BattleEnemyStatRecord *stats = &request->payload.loaded.stats;

    ((BattleContext *)gBattleContext)->active_object_resource =
        BattleObjectData_ResolveSlot(
            request->payload.loaded.stats.packed_object_data_id);
    ((BattleContext *)gBattleContext)->active_object_resource->read_size =
        sizeof(request->payload.loaded.object_data);
    ((BattleContext *)gBattleContext)->active_object_resource->destination =
        (u8 *)(stats + 1);
    BattleArchive_ReadAsync(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->active_object_resource, 0,
        ((BattleContext *)gBattleContext)
            ->active_object_resource->archive_cursor,
        ((BattleContext *)gBattleContext)
            ->active_object_resource->descriptor,
        0);
    task->callback = BattleEnemyData_FixupObjectPointers;
}

void BattleEnemyData_FixupObjectPointers(BattleQueuedTask *task) {
    BattleEnemyDataRequest *request =
        (BattleEnemyDataRequest *)task->argument;
    u8 *object_data = request->payload.bytes;

    request->stats = (BattleEnemyStatRecord *)object_data;
    object_data += sizeof(BattleEnemyStatRecord);
    request->object_data = object_data;
    request->object_data += *(u16 *)request->object_data;
    task->callback = 0;
}
