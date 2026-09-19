#include <game/scene_script.h>

typedef void (*SceneTaskVisitor)(SceneTask *task);

extern u8 *data_ov007_020a6b90;

extern void func_ov005_0206638c(int task_group, SceneTaskVisitor visitor);
extern void func_ov005_0206650c(SceneTask *task);

void SceneScriptTask_StopObject(SceneTask *task);

/* Metrowerks emits C functions in reverse source order. */
int SceneScript_IsReady(void) {
    return 1;
}

void SceneScriptTask_StopObject(SceneTask *task) {
    u16 stopped_object_id =
        *(u16 *)(data_ov007_020a6b90 + 0xA7E8);

    if (task->owner_id == stopped_object_id) {
        SceneScriptState *state = (SceneScriptState *)task->argument;

        if (state != 0) {
            state->vm_state.script = 0;
        }
        func_ov005_0206650c(task);
    }
}

void SceneScript_Stop(u32 owner) {
    switch (owner) {
    case SCENE_SCRIPT_OWNER_PRIMARY:
        *(const u16 **)(data_ov007_020a6b90 + 0x386C) = 0;
        return;
    case SCENE_SCRIPT_OWNER_SECONDARY:
        *(const u16 **)(data_ov007_020a6b90 + 0x3924) = 0;
        return;
    default:
        if ((owner & SCENE_SCRIPT_OWNER_TYPE_MASK) ==
            SCENE_SCRIPT_OWNER_OBJECT) {
            *(u16 *)(data_ov007_020a6b90 + 0xA7E8) =
                owner & SCENE_SCRIPT_OWNER_ID_MASK;
            func_ov005_0206638c(0, SceneScriptTask_StopObject);
        }
        return;
    }
}
