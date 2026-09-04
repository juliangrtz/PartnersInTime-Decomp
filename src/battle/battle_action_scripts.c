#include <game/battle_action.h>
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
    BATTLE_SPECIAL_MAP_ID = 0x2028,
    BATTLE_SPECIAL_STATE_OFFSET = 0x11344,
    BATTLE_SPECIAL_HANDLE_OFFSET = 0x11348
};

extern void *gBattleSystem;

typedef enum BattleSpecialMode {
    BATTLE_SPECIAL_MODE_ACTION_LOAD = 25
} BattleSpecialMode;

typedef struct BattleSpecialState {
    u8 unknown_00000[BATTLE_SPECIAL_STATE_OFFSET];
    BattleSpecialMode state;
    BattleSpecialMode handle;
} BattleSpecialState;

extern BattleSpecialState *data_ov002_020c0660;

int func_0202b6fc(void *system);
void func_0202b6e4(void *handle, int argument);
void func_ov002_0208e080(void *handle_slot);

void BattlePartyScript_FinishLoadTask(BattleQueuedTask *task);
void BattlePartyScript_LoadTask(BattleQueuedTask *task);
void BattleActionScript_FinishLoadTask(BattleQueuedTask *task);
void BattleActionScript_LoadTask(BattleQueuedTask *task);

BattleQueuedTask *BattleActionScript_RequestLoad(BattleActionState *action) {
    action->flags |= BATTLE_ACTION_FLAG_SCRIPT_LOAD_PENDING;
    if (*(s16 *)(gSaveData + SAVE_CURRENT_MAP_ID_OFFSET) ==
        BATTLE_SPECIAL_MAP_ID) {
        if (data_ov002_020c0660->handle != 0) {
            func_0202b6e4((void *)data_ov002_020c0660->handle, 0);
        }
        /* MWCC otherwise selects MOV for this pointer-like mode value; keep
         * the original literal load and register schedule localized here. */
        asm {
            ldr r2, =data_ov002_020c0660
            ldr r3, =BATTLE_SPECIAL_MODE_ACTION_LOAD
            ldr r0, [r2]
            ldr r1, =BATTLE_SPECIAL_HANDLE_OFFSET
            add r0, r0, #0x11000
            str r3, [r0, #0x344]
            ldr r0, [r2]
            add r0, r0, #0x11000
            ldr r3, [r0, #0x344]
            str r3, [r0, #0x348]
            ldr r0, [r2]
            add r0, r0, r1
            bl func_ov002_0208e080
        }
    }
    return BattleTaskQueue_Enqueue(BattleActionScript_LoadTask, action);
}

void BattleActionScript_LoadTask(BattleQueuedTask *task) {
    BattleActionState *action;

    if (*(s16 *)(gSaveData + SAVE_CURRENT_MAP_ID_OFFSET) ==
            BATTLE_SPECIAL_MAP_ID &&
        func_0202b6fc(gBattleSystem) != 0) {
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
