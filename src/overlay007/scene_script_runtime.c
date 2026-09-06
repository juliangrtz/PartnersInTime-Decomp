#include <game/scene_script.h>

typedef void (*SceneTaskCallback)(SceneTask *task);

extern u8 *data_ov007_020a6b90;

extern SceneScriptState *func_ov007_02089348(u32 object_id);
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
        return *(void **)(data_ov007_020a6b90 + 0x325C + object_id * 4);
    }
    return 0;
}

SceneScriptState *SceneScript_GetState(u32 owner) {
    switch (owner) {
    case SCENE_SCRIPT_OWNER_PRIMARY:
        return (SceneScriptState *)(data_ov007_020a6b90 + 0x386C);
    case SCENE_SCRIPT_OWNER_SECONDARY:
        return (SceneScriptState *)(data_ov007_020a6b90 + 0x3924);
    default:
        switch (owner & SCENE_SCRIPT_OWNER_TYPE_MASK) {
        case SCENE_SCRIPT_OWNER_OBJECT:
            return func_ov007_02089348(
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
    SceneScriptState *state = func_ov007_02089348(object_id);

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
                (ScriptVm *)(data_ov007_020a6b90 + 0x375C),
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
