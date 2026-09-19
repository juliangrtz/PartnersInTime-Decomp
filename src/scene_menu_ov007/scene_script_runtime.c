#include <game/script_vm.h>

typedef struct SceneScriptManager {
    u8 unknown_0000[0x37EC];
    s32 shared_variables[32];
} SceneScriptManager;

extern u16 data_0206032c[];
extern u16 data_ov007_020905f0[];
extern SceneScriptManager *data_ov007_020a6b90;

enum SceneScriptVariable {
    SCENE_SCRIPT_VAR_OWNER_ID = 0x7000,
    SCENE_SCRIPT_VAR_OWNER_TYPE = 0x7001,
    SCENE_SCRIPT_VAR_INPUT_0 = 0x7004,
    SCENE_SCRIPT_VAR_INPUT_1 = 0x7005,
    SCENE_SCRIPT_VAR_INPUT_3 = 0x7006,
    SCENE_SCRIPT_VAR_INPUT_0_WITH_LATCH = 0x7007,
    SCENE_SCRIPT_VAR_INPUT_1_WITH_LATCH = 0x7008,
    SCENE_SCRIPT_VAR_INPUT_3_WITH_LATCH = 0x7009,
    SCENE_SCRIPT_VAR_SHARED_FIRST = 0x7010,
    SCENE_SCRIPT_VAR_SHARED_LAST = 0x702F
};
/* Metrowerks emits C functions in reverse source order. */
s32 SceneScriptVM_ReadVariable(u16 variable, ScriptVm *vm,
                               ScriptVmState *state, u32 index) {
    switch (variable) {
    case SCENE_SCRIPT_VAR_OWNER_ID:
        return *(u16 *)((u8 *)state + 0xB0) & 0xFFF;
    case SCENE_SCRIPT_VAR_OWNER_TYPE:
        return (s32)*(u16 *)((u8 *)state + 0xB0) >> 12;
    case SCENE_SCRIPT_VAR_INPUT_0:
        return data_ov007_020905f0[0];
    case SCENE_SCRIPT_VAR_INPUT_1:
        return data_ov007_020905f0[2];
    case SCENE_SCRIPT_VAR_INPUT_3:
        return data_ov007_020905f0[1];
    case SCENE_SCRIPT_VAR_INPUT_0_WITH_LATCH:
        return data_0206032c[0] | data_ov007_020905f0[3];
    case SCENE_SCRIPT_VAR_INPUT_1_WITH_LATCH:
        return data_0206032c[1] | data_ov007_020905f0[4];
    case SCENE_SCRIPT_VAR_INPUT_3_WITH_LATCH:
        return data_0206032c[3] | data_ov007_020905f0[6];
    default:
        if (variable >= SCENE_SCRIPT_VAR_SHARED_FIRST
            && variable <= SCENE_SCRIPT_VAR_SHARED_LAST) {
            return data_ov007_020a6b90
                ->shared_variables[variable - SCENE_SCRIPT_VAR_SHARED_FIRST];
        }
        return 0;
    }
}

void SceneScriptVM_WriteVariable(u16 variable, s32 value, ScriptVm *vm,
                                 ScriptVmState *state) {
    if (variable >= SCENE_SCRIPT_VAR_SHARED_FIRST
        && variable <= SCENE_SCRIPT_VAR_SHARED_LAST) {
        data_ov007_020a6b90
            ->shared_variables[variable - SCENE_SCRIPT_VAR_SHARED_FIRST] = value;
    }
}

#include <game/scene_script.h>

typedef void (*SceneTaskCallback)(SceneTask *task);

extern SceneScriptState *SceneScript_GetObjectState(u32 object_id);
extern SceneTask *func_ov005_0206659c(
    SceneTaskCallback callback, int task_group, int priority
);
extern void func_ov005_0206650c(SceneTask *task);

int SceneScriptTask_HandleResult(
    SceneTask *task, int result, SceneScriptState *state
);
void SceneScriptTask_Update(SceneTask *task);

/* Metrowerks emits C functions in reverse source order. */
void *SceneObject_GetById(u32 object_id) {
    if (object_id < 56) {
        return *(void **)((u8 *)data_ov007_020a6b90 + 0x325C + object_id * 4);
    }
    return 0;
}

SceneScriptState *SceneScript_GetState(u32 owner) {
    switch (owner) {
    case SCENE_SCRIPT_OWNER_PRIMARY:
        return (SceneScriptState *)((u8 *)data_ov007_020a6b90 + 0x386C);
    case SCENE_SCRIPT_OWNER_SECONDARY:
        return (SceneScriptState *)((u8 *)data_ov007_020a6b90 + 0x3924);
    default:
        switch (owner & SCENE_SCRIPT_OWNER_TYPE_MASK) {
        case SCENE_SCRIPT_OWNER_OBJECT:
            return SceneScript_GetObjectState(
                owner & SCENE_SCRIPT_OWNER_ID_MASK
            );
        default:
            return 0;
        }
    }
}

void SceneScript_StartObjectScript(
    u32 object_id,
    const u16 *script,
    const SceneScriptState *parent,
    int queue_if_running
) {
    SceneScriptState *state = SceneScript_GetObjectState(object_id);

    if (state->vm_state.script != 0 && queue_if_running) {
        if (parent->owner >= SCENE_SCRIPT_OWNER_OBJECT) {
            state->queued_parent_owner = parent->parent_owner;
            state->queued_parent_object_id =
                parent->owner & SCENE_SCRIPT_OWNER_ID_MASK;
        } else {
            state->queued_parent_owner = parent->owner;
            state->queued_parent_object_id = -1;
        }
        state->queued_script = script;
        return;
    }

    if (state->vm_state.script != 0) {
        SceneScript_Stop(object_id | SCENE_SCRIPT_OWNER_OBJECT);
    }
    if (parent->owner >= SCENE_SCRIPT_OWNER_OBJECT) {
        state->parent_owner = parent->parent_owner;
        state->parent_object_id = parent->owner & SCENE_SCRIPT_OWNER_ID_MASK;
    } else {
        state->parent_owner = parent->owner;
        state->parent_object_id = -1;
    }
    state->vm_state.script = script;
    state->vm_state.stack_depth = 0;
    state->vm_state.delay = 0;
    state->flags.bits.paused = 0;
    state->queued_script = 0;
    state->queued_parent_owner = 0;
    state->queued_parent_object_id = -1;
    state->owner = object_id | SCENE_SCRIPT_OWNER_OBJECT;

    {
        SceneTask *task = func_ov005_0206659c(
            SceneScriptTask_Update, 0, 1
        );
        task->argument = state;
        task->owner_id = object_id;
    }
}

void SceneScriptTask_Update(SceneTask *task) {
    SceneScriptState *state = (SceneScriptState *)task->argument;
    if (state->vm_state.script == 0) {
        func_ov005_0206650c(task);
        state->parent_owner = 0;
        state->parent_object_id = -1;
        state->queued_script = 0;
        state->queued_parent_owner = 0;
        state->queued_parent_object_id = -1;
        return;
    }
    if (state->flags.bits.paused != 0) {
        return;
    }
    do {
        int result = VM_Run(
                (ScriptVm *)((u8 *)data_ov007_020a6b90 + 0x375C),
                &state->vm_state
            );
        if (!SceneScriptTask_HandleResult(task, result, state)) {
            break;
        }
    } while (1);
}

int SceneScriptTask_HandleResult(
    SceneTask *task, int result, SceneScriptState *state
) {
    switch (result) {
    case SCRIPT_VM_RETURNED:
        if (state->queued_script != 0) {
            state->vm_state.script = state->queued_script;
            state->parent_owner = state->queued_parent_owner;
            state->parent_object_id = state->queued_parent_object_id;
            state->queued_script = 0;
            state->queued_parent_owner = 0;
            state->queued_parent_object_id = -1;
            return 1;
        }
        /* A returned script without a queued successor is finished too. */
    case SCRIPT_VM_FINISHED:
        func_ov005_0206650c(task);
        state->parent_owner = 0;
        state->parent_object_id = -1;
        state->queued_script = 0;
        state->queued_parent_owner = 0;
        state->queued_parent_object_id = -1;
        break;
    case SCRIPT_VM_YIELDED:
        break;
    }
    return 0;
}
