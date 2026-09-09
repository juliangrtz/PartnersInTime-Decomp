#include <game/battle_object.h>
#include <game/battle_task_queue.h>

BattleQueuedTask *BattleObjectData_QueueLoad(
    BattleObjectDataLoadState *load_state, s32 resource_id) {
    load_state->resource_id = resource_id;
    load_state->component_08 = 0;
    load_state->flags.bits.processing = 1;
    load_state->flags.bits.load_pending = 0;
    return BattleTaskQueue_Enqueue(
        BattleObjectData_ResolveTextureMetadataTask, load_state);
}
