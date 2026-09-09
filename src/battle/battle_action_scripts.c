#include <game/battle_action.h>
#include <game/battle_attack_loader.h>
#include <game/linker_overlay_ids.h>
#include <game/battle_ai.h>
#include <game/battle_archive.h>
#include <game/battle_context.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>
#include <game/save_data.h>

enum BattleActionScriptConstant {
    SAVE_CURRENT_MAP_ID_OFFSET = 0x55A,
    BATTLE_ACTION_SCRIPT_BUFFER_OFFSET = 0x25608,
    BATTLE_PARTY_SCRIPT_BUFFER_OFFSET = 0x35608,
    BATTLE_SPECIAL_MAP_ID = 0x2028
};

extern void *gBattleSystem;

int ArchiveIO_FinishOverlay(void *system);
void ArchiveIO_UnloadOverlay(void *handle, int argument);
void BattleSpecialHandle_QueueReload(void *handle_slot);

void BattlePartyScript_FinishLoadTask(BattleQueuedTask *task);
void BattlePartyScript_LoadTask(BattleQueuedTask *task);
void BattleActionScript_FinishLoadTask(BattleQueuedTask *task);
void BattleActionScript_LoadTask(BattleQueuedTask *task);

BattleQueuedTask *BattleActionScript_RequestLoad(BattleActionState *action) {
    action->flags |= BATTLE_ACTION_FLAG_SCRIPT_LOAD_PENDING;
    if (*(s16 *)(gSaveData + SAVE_CURRENT_MAP_ID_OFFSET) ==
        BATTLE_SPECIAL_MAP_ID) {
        if (data_ov002_020c0660->active_overlay != 0) {
            ArchiveIO_UnloadOverlay(data_ov002_020c0660->active_overlay, 0);
        }
        data_ov002_020c0660->requested_overlay = OVERLAY_25_ID;
        data_ov002_020c0660->active_overlay = data_ov002_020c0660->requested_overlay;
        BattleSpecialHandle_QueueReload(&data_ov002_020c0660->active_overlay);
    }
    return BattleTaskQueue_Enqueue(BattleActionScript_LoadTask, action);
}

void BattleActionScript_LoadTask(BattleQueuedTask *task) {
    BattleActionState *action;

    if (*(s16 *)(gSaveData + SAVE_CURRENT_MAP_ID_OFFSET) ==
            BATTLE_SPECIAL_MAP_ID &&
        ArchiveIO_FinishOverlay(gBattleSystem) != 0) {
        return;
    }

    action = (BattleActionState *)task->argument;
    ((BattleContext *)gBattleContext)->active_object_resource =
        BattleObjectData_ResolveSlot(
            action->definition->resource_id->packed_object_data_id);
    ((BattleContext *)gBattleContext)->active_object_resource->read_size =
        (BattleArchive_GetEntrySize(
             gBattleSystem,
             ((BattleContext *)gBattleContext)
                 ->active_object_resource->archive_cursor,
             ((BattleContext *)gBattleContext)
                 ->active_object_resource->entry_index) + 3) & ~3;
    ((BattleContext *)gBattleContext)->active_object_resource->destination =
        gBattleContext + BATTLE_ACTION_SCRIPT_BUFFER_OFFSET;
    BattleArchive_ReadAsync(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->active_object_resource, 0,
        ((BattleContext *)gBattleContext)
            ->active_object_resource->archive_cursor,
        ((BattleContext *)gBattleContext)->active_object_resource->descriptor,
        0);
    task->callback = BattleActionScript_FinishLoadTask;
}

void BattleActionScript_FinishLoadTask(BattleQueuedTask *task) {
    BattleContext *context = (BattleContext *)gBattleContext;
    BattleActionState *action;
    u8 *script_buffer;

    if ((context->active_object_resource->status & 3) != 0) {
        return;
    }

    action = (BattleActionState *)task->argument;
    BattleAI_StartActionScript(action->scene_object->actor_id);
    script_buffer = gBattleContext + BATTLE_ACTION_SCRIPT_BUFFER_OFFSET;
    action->script = (const u16 *)(script_buffer + *(u16 *)script_buffer);
    action->flags &= ~BATTLE_ACTION_FLAG_SCRIPT_LOAD_PENDING;
    task->callback = 0;
}

BattleQueuedTask *BattlePartyScript_RequestLoad(u16 packed_object_data_id) {
    return BattleTaskQueue_Enqueue(BattlePartyScript_LoadTask,
                                   (void *)packed_object_data_id);
}

void BattlePartyScript_LoadTask(BattleQueuedTask *task) {
    ((BattleContext *)gBattleContext)->active_object_resource =
        BattleObjectData_ResolveSlot((u16)(u32)task->argument);
    ((BattleContext *)gBattleContext)->active_object_resource->destination =
        gBattleContext + BATTLE_PARTY_SCRIPT_BUFFER_OFFSET;
    ((BattleContext *)gBattleContext)->active_object_resource->read_size =
        (BattleArchive_GetEntrySize(
             gBattleSystem,
             ((BattleContext *)gBattleContext)
                 ->active_object_resource->archive_cursor,
             ((BattleContext *)gBattleContext)
                 ->active_object_resource->entry_index) + 3) & ~3;
    BattleArchive_ReadAsync(
        gBattleSystem,
        ((BattleContext *)gBattleContext)->active_object_resource, 0,
        ((BattleContext *)gBattleContext)
            ->active_object_resource->archive_cursor,
        ((BattleContext *)gBattleContext)->active_object_resource->descriptor,
        0);
    task->callback = BattlePartyScript_FinishLoadTask;
}

void BattlePartyScript_FinishLoadTask(BattleQueuedTask *task) {
    task->callback = 0;
    BattleAI_StartPartyVmSlot4();
}
