#include <game/battle_context.h>
#include <game/battle_enemy_data.h>
#include <game/battle_object.h>
#include <game/battle_task_queue.h>

extern void OS_Terminate(void);
extern void *func_02029928(u32 heap_id, u32 size, const void *name,
                          int use_default);
extern int func_0202a914(char *destination, const char *format, ...);
extern void func_0202cbd4(void *destination, int value, u32 size);
extern BattleQueuedTask *func_ov002_0208bd38(
    BattleObjectDataLoadState *load_state, s32 resource_id);
extern const char data_ov002_020bf724[];

enum BattleObjectDataSlotRange {
    BATTLE_OBJECT_DATA_SLOT_COUNT = 72,
    BATTLE_ENEMY_DATA_SLOT_COUNT = 8,
    BATTLE_RESOURCE_SLOT_COUNT =
        BATTLE_OBJECT_DATA_SLOT_COUNT + BATTLE_ENEMY_DATA_SLOT_COUNT,
    BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET = 0xD3C8,
    BATTLE_OBJECT_DATA_HEAP_GROUP_0_END = 20,
    BATTLE_OBJECT_DATA_HEAP_GROUP_1_END = 36,
    BATTLE_OBJECT_DATA_HEAP_GROUP_2_END = 52,
    BATTLE_OBJECT_DATA_LARGE_SLOT = 51,
    BATTLE_OBJECT_DATA_LARGE_SLOT_SIZE = 0x1800,
    BATTLE_OBJECT_DATA_STREAM_HEADER_SIZE = 100,
    BATTLE_OBJECT_DATA_ALLOCATION_NAME_SIZE = 36
};

void BattleObjectData_AllocateLoadBuffer(int object_data_id, u32 size) {
    u32 heap_id;
    u32 offset;
    BattleObjectDataLoadState *load_state;
    void *buffer;
    char allocation_name[BATTLE_OBJECT_DATA_ALLOCATION_NAME_SIZE];

    if ((u32)object_data_id < BATTLE_OBJECT_DATA_HEAP_GROUP_0_END) {
        heap_id = ((BattleContext *)gBattleContext)
                      ->runtime.object_data_heap_ids[0];
    } else if ((u32)object_data_id < BATTLE_OBJECT_DATA_HEAP_GROUP_1_END) {
        heap_id = ((BattleContext *)gBattleContext)
                      ->runtime.object_data_heap_ids[1];
    } else if ((u32)object_data_id < BATTLE_OBJECT_DATA_HEAP_GROUP_2_END) {
        heap_id = ((BattleContext *)gBattleContext)
                      ->runtime.object_data_heap_ids[2];
    } else if ((u32)object_data_id < BATTLE_OBJECT_DATA_SLOT_COUNT) {
        heap_id = ((BattleContext *)gBattleContext)
                      ->runtime.object_data_heap_ids[3];
    }

    offset = object_data_id * sizeof(BattleObjectDataLoadState);
    if (*(void **)(gBattleContext +
                   BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET + offset) != 0) {
        OS_Terminate();
    }

    if (object_data_id == BATTLE_OBJECT_DATA_LARGE_SLOT) {
        size = BATTLE_OBJECT_DATA_LARGE_SLOT_SIZE;
    }

    func_0202a914(allocation_name, data_ov002_020bf724, object_data_id);
    load_state = BattleObjectData_GetLoadState(object_data_id);
    load_state->data = func_02029928(heap_id, size, allocation_name, 1);

    if (size > BATTLE_OBJECT_DATA_STREAM_HEADER_SIZE) {
        buffer = *(void **)(
            gBattleContext + BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET +
            offset);
        func_0202cbd4(buffer, 0, BATTLE_OBJECT_DATA_STREAM_HEADER_SIZE);
        load_state->stream_state = buffer;
        *(u8 **)buffer =
            (u8 *)buffer + BATTLE_OBJECT_DATA_STREAM_HEADER_SIZE;
    } else {
        load_state->stream_state = 0;
    }

    load_state->flags.raw &= 0xF8000000;
    load_state->flags.bits.allocated = 0;
    load_state->allocation_size = size;
    load_state->flags.bits.upload_complete = 0;
    load_state->flags.bits.copy_flag = 1;
}

void BattleObjectData_ConfigureLoad(int object_data_id, int resource_index,
                                    int compressed) {
    if ((u32)object_data_id >= BATTLE_OBJECT_DATA_SLOT_COUNT) {
        OS_Terminate();
    }

    ((BattleContext *)gBattleContext)
        ->runtime.object_data_load_states[object_data_id]
        .flags.bits.resource_index = resource_index + 1;
    ((BattleContext *)gBattleContext)
        ->runtime.object_data_load_states[object_data_id]
        .flags.bits.upload_complete = compressed;
}

int BattleObjectData_EnsureLoaded(u16 object_data_id, s32 resource_id) {
    BattleObjectDataLoadState *load_state;
    BattleQueuedTask *task;
    u32 offset;

    if (object_data_id >= BATTLE_RESOURCE_SLOT_COUNT) {
        OS_Terminate();
    }

    if (object_data_id < BATTLE_OBJECT_DATA_SLOT_COUNT) {
        offset = object_data_id * sizeof(BattleObjectDataLoadState);
        if (*(void **)(gBattleContext +
                       BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET + offset) ==
            0) {
            OS_Terminate();
        }

        load_state = BattleObjectData_GetLoadState(object_data_id);
        if (resource_id == load_state->resource_id) {
            return load_state->resource_id;
        }

        load_state =
            (BattleObjectDataLoadState *)(
                gBattleContext + BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET +
                offset);
        task = BattleObjectData_QueueLoad(load_state, resource_id);
        task->state = object_data_id;
        return (int)task;
    }

    return (int)BattleEnemyData_RequestLoad(
        &((BattleContext *)gBattleContext)
             ->enemy_data_requests[object_data_id -
                                   BATTLE_OBJECT_DATA_SLOT_COUNT],
        resource_id);
}

BattleQueuedTask *func_ov002_02091fd8(int object_data_id, s32 resource_id) {
    BattleObjectDataLoadState *load_state;
    BattleQueuedTask *task;
    u32 offset;

    if ((u32)object_data_id >= BATTLE_OBJECT_DATA_SLOT_COUNT) {
        OS_Terminate();
    }

    offset = object_data_id * sizeof(BattleObjectDataLoadState);
    if (*(void **)(gBattleContext +
                   BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET + offset) == 0) {
        OS_Terminate();
    }

    load_state =
        (BattleObjectDataLoadState *)(
            gBattleContext + BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET +
            offset);
    task = BattleObjectData_QueueLoadAndMarkPending(load_state, resource_id);
    task->state = object_data_id;
    return task;
}

BattleQueuedTask *func_ov002_02091f68(int object_data_id, s32 resource_id) {
    BattleObjectDataLoadState *load_state;
    BattleQueuedTask *task;
    u32 offset;

    if ((u32)object_data_id >= BATTLE_OBJECT_DATA_SLOT_COUNT) {
        OS_Terminate();
    }

    offset = object_data_id * sizeof(BattleObjectDataLoadState);
    if (*(void **)(gBattleContext +
                   BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET + offset) == 0) {
        OS_Terminate();
    }

    load_state =
        (BattleObjectDataLoadState *)(
            gBattleContext + BATTLE_OBJECT_DATA_LOAD_STATE_TABLE_OFFSET +
            offset);
    task = func_ov002_0208bd38(load_state, resource_id);
    task->state = object_data_id;
    return task;
}

int BattleObjectData_IsLoadPending(int object_data_id) {
    if ((u32)object_data_id >= BATTLE_RESOURCE_SLOT_COUNT) {
        OS_Terminate();
    }

    if ((u32)object_data_id < BATTLE_OBJECT_DATA_SLOT_COUNT) {
        BattleObjectDataLoadState *load_state =
            &((BattleContext *)gBattleContext)
                 ->runtime.object_data_load_states[object_data_id];

        if (!load_state->flags.bits.processing) {
            return 0;
        }
    } else {
        BattleEnemyDataRequest *request =
            &((BattleContext *)gBattleContext)
                 ->enemy_data_requests[object_data_id -
                                       BATTLE_OBJECT_DATA_SLOT_COUNT];

        if (request->stats != 0) {
            return 0;
        }
    }

    return 1;
}
