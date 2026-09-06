#include <game/scene_script.h>

typedef struct SceneManagerWork {
    u8 unknown_000[0x25C];
    SceneObject *first_object;
    u8 unknown_260[0x3DC];
    void *task_slots[72];
} SceneManagerWork;

typedef struct SceneManagerCleanupLayout {
    u8 unknown_0000[0x3000];
    SceneManagerWork work;
    u8 unknown_375c[0x2080];
    void *primary_script_archive;
    u8 unknown_57e0[0x5000];
    void *temporary_archive;
    u16 unknown_a7e4;
    u16 unknown_a7e6;
    u16 running_task_owner;
} SceneManagerCleanupLayout;

extern u8 *data_ov007_020a6b90;

extern void func_02029ba4(void *allocation);
extern void func_0202cbd4(void *destination, u32 value, u32 size);
extern void func_ov005_020663d8(int group);
extern void func_ov005_02068c54(void *renderable);
extern void func_ov005_020698f0(int slot);
extern void func_ov007_0208701c(SceneObject *object);
extern void func_ov007_020894c4(void *renderable);
extern SceneScriptState *func_ov007_02089348(u32 object_id);

/* Metrowerks emits C functions in reverse source order. */
void SceneManager_ClearRuntime(void *manager_raw) {
    SceneManagerCleanupLayout *manager =
        (SceneManagerCleanupLayout *)manager_raw;
    SceneObject *object;
    int object_id;
    int slot;

    *(u16 *)(data_ov007_020a6b90 + 0xA7EA) = 0;
    SceneScript_Stop(SCENE_SCRIPT_OWNER_PRIMARY);
    SceneScript_Stop(SCENE_SCRIPT_OWNER_SECONDARY);
    func_ov005_020663d8(0);

    object = manager->work.first_object;
    for (object_id = 0; object_id < 56; object_id++) {
        func_ov007_0208701c(object);
        if (object->primary_renderable != 0) {
            func_ov007_020894c4(object->primary_renderable);
            if (object->primary_renderable != 0) {
                func_ov005_02068c54(object->primary_renderable);
                object->primary_renderable = 0;
            }
        }
        if (object->secondary_renderable != 0) {
            func_ov007_020894c4(object->secondary_renderable);
            if (object->secondary_renderable != 0) {
                func_ov005_02068c54(object->secondary_renderable);
                object->secondary_renderable = 0;
            }
        }
        object++;
    }

    for (slot = 0; slot < 72; slot++) {
        manager->work.task_slots[slot] = 0;
        func_ov005_020698f0(slot);
    }

    if (manager->temporary_archive != 0) {
        func_02029ba4(manager->temporary_archive);
        manager->temporary_archive = 0;
    }
    if (manager->primary_script_archive != 0) {
        func_02029ba4(manager->primary_script_archive);
        manager->primary_script_archive = 0;
    }
}

void SceneObjects_ClearSecondaryRange(u8 *manager) {
    SceneManagerCleanupLayout *scene = (SceneManagerCleanupLayout *)manager;
    SceneObject *object;
    int object_id;
    int slot;

    SceneScript_Stop(SCENE_SCRIPT_OWNER_SECONDARY);
    for (object_id = 40; object_id < 56; object_id++) {
        object = (SceneObject *)SceneObject_GetById(object_id);
        func_ov007_0208701c(object);
        if (object->primary_renderable != 0) {
            func_ov007_020894c4(object->primary_renderable);
            if (object->primary_renderable != 0) {
                func_ov005_02068c54(object->primary_renderable);
                object->primary_renderable = 0;
            }
        }
        if (object->secondary_renderable != 0) {
            func_ov007_020894c4(object->secondary_renderable);
            if (object->secondary_renderable != 0) {
                func_ov005_02068c54(object->secondary_renderable);
                object->secondary_renderable = 0;
            }
        }
        func_0202cbd4(object, 0, sizeof(SceneObject));
        object->object_id = object_id;
        func_ov007_02089348(object_id)->vm_state.script = 0;
    }
    for (slot = 52; slot < 72; slot++) {
        scene->work.task_slots[slot] = 0;
        func_ov005_020698f0(slot);
    }
}
