/*
 * Scene manager construction (overlay 7, 0x020848F8-0x02084FBC).
 *
 * Builds the block the scene subsystem runs out of: loads the common archives
 * and the entries they contain, then hands the same allocation to the runtime
 * initializer, which sees it through a different view because the two describe
 * different parts of it. data_ov007_020a6b90 is set here and is how the rest of
 * overlay 7 finds the scene.
 */

#include <game/scene_script.h>

typedef struct SceneResourceManagerLayout {
    u8 unknown_0000[0x60];
    void *common_archives[6];
    u8 unknown_0078[0x35C4];
    void *task_slots[72];
    u8 unknown_375c[0x70D0];
    void *controller_task;
} SceneResourceManagerLayout;

extern u8 data_ov005_0206a180[];
extern u8 *data_ov007_020a6b90;
extern const void *data_ov007_0208dbbc[];
extern const void *data_ov007_0208dc8c[];

extern void MI_CpuFill8(void *destination, u32 value, u32 size);
extern void func_ov005_02067468(
    void *archive_manager, u8 archive_slot, const void *archive_name,
    int load_immediately);
extern void *Overlay5Archive_ReadEntry(
    void *archive_manager, u8 archive_slot, int member_id,
    void *size_output, int allocate_from_end);
extern void func_ov007_0208a5cc(void);

/* The loader and the runtime initializer describe the same allocation through
 * different views; SceneManagerInitLayout is defined further down, next to the
 * initializer that uses it. */
struct SceneManagerInitLayout;
extern void SceneManager_InitializeRuntime(struct SceneManagerInitLayout *manager);

SceneResourceManagerLayout *SceneManager_LoadResources(
    SceneResourceManagerLayout *manager) {
    int archive_index;

    data_ov007_020a6b90 = (u8 *)manager;
    func_ov005_02067468(
        *(void **)(data_ov005_0206a180 + 0x2C), 4,
        data_ov007_0208dbbc[0], 1);
    for (archive_index = 0; archive_index < 6; archive_index++) {
        func_ov005_02067468(
            *(void **)(data_ov005_0206a180 + 0x2C),
            archive_index + 6, data_ov007_0208dc8c[archive_index], 1);
        manager->common_archives[archive_index] = Overlay5Archive_ReadEntry(
            *(void **)(data_ov005_0206a180 + 0x2C),
            archive_index + 6, 0, 0, 1);
    }
    manager->controller_task = 0;
    MI_CpuFill8(
        manager->task_slots, 0, sizeof(manager->task_slots));
    func_ov007_0208a5cc();
    SceneManager_InitializeRuntime((struct SceneManagerInitLayout *)manager);
    return manager;
}

typedef struct SceneControllerTask {
    u8 unknown_00[0x20];
    u32 state;
    u32 unknown_24;
    int argument;
} SceneControllerTask;

typedef struct SceneControllerManagerLayout {
    u8 unknown_0000[0x60];
    void *common_archives[6];
    u8 unknown_0078[0xA7B4];
    SceneControllerTask *controller_task;
} SceneControllerManagerLayout;

extern void GameHeap_DeleteArray(void *allocation);
extern void func_ov005_0206650c(SceneControllerTask *task);
extern SceneControllerTask *func_ov005_0206659c(
    void (*callback)(SceneControllerTask *), int group, int priority);
extern int func_ov005_02066920(
    void *archive_manager, int archive_slot, int member_id,
    void *destination, int argument_4, int argument_5);
extern int func_ov005_02067424(void *archive_manager, int archive_slot);
extern void SceneWindow_Destroy(void);
extern void SceneTransitionController_Update(SceneControllerTask *task);

/* Metrowerks emits C functions in reverse source order. */
SceneControllerManagerLayout *SceneManager_Shutdown(
    SceneControllerManagerLayout *manager) {
    if (manager->controller_task != 0) {
        func_ov005_0206650c(manager->controller_task);
        manager->controller_task = 0;
    }
    SceneManager_ClearRuntime(manager);
    SceneWindow_Destroy();

    if (manager->common_archives[5] != 0) {
        GameHeap_DeleteArray(manager->common_archives[5]);
        manager->common_archives[5] = 0;
    }
    if (manager->common_archives[4] != 0) {
        GameHeap_DeleteArray(manager->common_archives[4]);
        manager->common_archives[4] = 0;
    }
    if (manager->common_archives[3] != 0) {
        GameHeap_DeleteArray(manager->common_archives[3]);
        manager->common_archives[3] = 0;
    }
    if (manager->common_archives[2] != 0) {
        GameHeap_DeleteArray(manager->common_archives[2]);
        manager->common_archives[2] = 0;
    }
    if (manager->common_archives[1] != 0) {
        GameHeap_DeleteArray(manager->common_archives[1]);
        manager->common_archives[1] = 0;
    }
    if (manager->common_archives[0] != 0) {
        GameHeap_DeleteArray(manager->common_archives[0]);
        manager->common_archives[0] = 0;
    }
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 11);
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 10);
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 9);
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 8);
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 7);
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 6);
    func_ov005_02067424(
        *(void **)(data_ov005_0206a180 + 0x2C), 4);
    return manager;
}

void SceneController_Create(u8 *manager) {
    SceneControllerManagerLayout *scene =
        (SceneControllerManagerLayout *)manager;

    scene->controller_task = func_ov005_0206659c(
        SceneTransitionController_Update, 0, 1);
    scene->controller_task->state = 0;
}

void SceneController_Start(u8 *manager, int argument) {
    SceneControllerManagerLayout *scene =
        (SceneControllerManagerLayout *)manager;

    if (scene->controller_task != 0) {
        SceneScript_Stop(SCENE_SCRIPT_OWNER_SECONDARY);
        scene->controller_task->state = 1;
        scene->controller_task->argument = argument;
    }
}

void SceneController_Stop(u8 *manager) {
    SceneControllerManagerLayout *scene =
        (SceneControllerManagerLayout *)manager;

    if (scene->controller_task != 0) {
        SceneScript_Stop(SCENE_SCRIPT_OWNER_SECONDARY);
        scene->controller_task->state = 4;
    }
}

int SceneController_IsObjectReady(const u8 *manager) {
    const SceneControllerManagerLayout *scene =
        (const SceneControllerManagerLayout *)manager;
    SceneObject *object;

    if (scene->controller_task == 0) {
        return 0;
    }
    object = (SceneObject *)SceneObject_GetById(17);
    if (object->secondary_renderable == 0) {
        return 0;
    }
    /*
     * The original compiler retained this otherwise redundant bitfield probe.
     * Preserve the seven-instruction tail so the reconstructed function stays
     * byte-identical while exposing its effective result: x >= 0.
     */
    asm {
        ldrh r1, [r0, #0xe0]
        ldrsh r2, [r0, #4]
        mov r0, r1, lsl #28
        mov r0, r0, lsr #31
        cmp r0, #1
        cmp r2, #0
        movlt r0, #0
        movge r0, #1
    }
}

int SceneTask_LoadArchiveMember(SceneTask *task) {
    return func_ov005_02066920(
        *(void **)(data_ov005_0206a180 + 0x2C),
        8, 3, *(void **)((u8 *)task + 0x68), 0, 0);
}

typedef struct SceneVmRuntime {
    u32 unknown_00;
    u32 unknown_04;
    void *command_handler;
    const void *opcode_table;
} SceneVmRuntime;

typedef struct SceneManagerInitLayout {
    u8 unknown_0000[0x7C];
    SceneObject objects[56];
    SceneObject *object_slots[56];
    u8 unknown_333c[0x420];
    SceneVmRuntime vm;
    u8 unknown_376c[0x80];
    u8 shared_variables[0x80];
    u8 primary_script[0xB8];
    u8 secondary_script[0xB8];
    SceneScriptState object_scripts[40];
    void *primary_archive;
    u8 secondary_archive[0x5000];
    void *temporary_archive;
    u8 unknown_a7e4[0x08];
    u8 task_script_state[0x40];
} SceneManagerInitLayout;

extern const u8 data_ov007_0208dca4[];

extern void MIi_CpuClear16(u16 value, void *destination, u32 size);
void SceneManager_InitializeRuntime(SceneManagerInitLayout *manager) {
    int object_id;
    int script_id;
    u32 *object_metadata;
    SceneObject *object;
    SceneObject **slot;
    volatile u16 clear_value;

    manager->temporary_archive = 0;
    manager->primary_archive = 0;
    MI_CpuFill8(manager->primary_script, 0, 0xB8);
    MI_CpuFill8(manager->secondary_script, 0, 0xB8);
    MI_CpuFill8(manager->shared_variables, 0, 0x80);

    clear_value = 0;
    MIi_CpuClear16(clear_value, manager->objects, sizeof(manager->objects));
    object_metadata = (u32 *)manager;
    object = manager->objects;
    object_id = 0;
    do {
        *(u16 *)((u8 *)object_metadata + 0x158) = object_id;
        slot = &manager->object_slots[object_id++];
        *(u16 *)((u8 *)object_metadata + 0x15A) =
            *(u16 *)((u8 *)object_metadata + 0x158);
        *slot = object;
        object_metadata += sizeof(SceneObject) / sizeof(u32);
        object++;
    } while (object_id < 56);

    MI_CpuFill8(
        manager->object_scripts, 0, sizeof(manager->object_scripts));
    for (script_id = 0; script_id < 40; script_id++) {
        manager->object_scripts[script_id].vm_state.script = 0;
    }

    MI_CpuFill8(
        manager->task_script_state, 0, sizeof(manager->task_script_state));
    manager->vm.unknown_00 = 0;
    manager->vm.unknown_04 = 0;
    manager->vm.opcode_table = data_ov007_0208dca4;
    manager->vm.command_handler = SceneVm_DispatchCommand;
}

#include <game/scene_motion.h>

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

extern void func_0202cbd4(void *destination, u32 value, u32 size);
extern void func_ov005_020663d8(int group);
extern void func_ov005_02068c54(void *renderable);
extern void func_ov005_020698f0(int slot);
extern void SceneModel_Stop(void *renderable);
extern SceneScriptState *SceneScript_GetObjectState(u32 object_id);

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
        object++;
    }

    for (slot = 0; slot < 72; slot++) {
        manager->work.task_slots[slot] = 0;
        func_ov005_020698f0(slot);
    }

    if (manager->temporary_archive != 0) {
        GameHeap_DeleteArray(manager->temporary_archive);
        manager->temporary_archive = 0;
    }
    if (manager->primary_script_archive != 0) {
        GameHeap_DeleteArray(manager->primary_script_archive);
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
        SceneScript_GetObjectState(object_id)->vm_state.script = 0;
    }
    for (slot = 52; slot < 72; slot++) {
        scene->work.task_slots[slot] = 0;
        func_ov005_020698f0(slot);
    }
}
