#include <game/scene_motion.h>

typedef struct SceneManagerWork {
    u8 unknown_000[0x25C];
    SceneObject *first_object;
    u8 unknown_260[0x0DC];
    SceneObject *render_rows[192];
    void *task_slots[72];
} SceneManagerWork;

typedef struct SceneManager {
    u8 unknown_0000[0x3000];
    SceneManagerWork work;
} SceneManager;

extern u8 *data_ov007_020a6b90;

extern void func_0202cbd4(void *destination, u32 value, u32 size);
extern void func_ov005_02068c54(void *renderable);
extern void func_ov005_020698f0(int slot);
extern void SceneModel_Stop(void *renderable);
extern SceneScriptState *SceneScript_GetObjectState(u32 object_id);

/* Metrowerks emits C functions in reverse source order. */
void SceneObjects_ClearPrimaryRange(u8 *manager) {
    SceneObject *object;
    int object_id;
    int slot;
    SceneManager *scene = (SceneManager *)manager;

    SceneScript_Stop(SCENE_SCRIPT_OWNER_PRIMARY);
    for (object_id = 28; object_id < 40; object_id++) {
        object = (SceneObject *)SceneObject_GetById(object_id);

        SceneObject_UnlinkMotion(object);
        if (object->primary_renderable != 0) {
            SceneModel_Stop(object->primary_renderable);
            if (object->primary_renderable != 0) {
                func_ov005_02068c54(object->primary_renderable);
                object->primary_renderable = 0;
            }
        }
        if (object->secondary_renderable != 0) {
            SceneModel_Stop(object->secondary_renderable);
            if (object->secondary_renderable != 0) {
                func_ov005_02068c54(object->secondary_renderable);
                object->secondary_renderable = 0;
            }
        }
        func_0202cbd4(object, 0, sizeof(SceneObject));
        object->object_id = object_id;
    }
    for (object_id = 28; object_id < 40; object_id++) {
        SceneScript_GetObjectState(object_id)->vm_state.script = 0;
    }
    for (slot = 36; slot < 52; slot++) {
        scene->work.task_slots[slot] = 0;
        func_ov005_020698f0(slot);
    }
}

void SceneScripts_UpdateGlobal(u8 *manager) {
    if (*(const u16 **)(manager + 0x386C) != 0) {
        VM_Run(
            (ScriptVm *)(manager + 0x375C),
            (ScriptVmState *)(manager + 0x386C)
        );
    }
    if (*(const u16 **)(manager + 0x3924) != 0) {
        VM_Run(
            (ScriptVm *)(manager + 0x375C),
            (ScriptVmState *)(manager + 0x3924)
        );
    }
}
