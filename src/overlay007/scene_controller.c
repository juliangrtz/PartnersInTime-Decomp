#include <game/scene_script.h>

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

extern u8 data_ov005_0206a180[];

extern void func_02029ba4(void *allocation);
extern void func_ov005_0206650c(SceneControllerTask *task);
extern SceneControllerTask *func_ov005_0206659c(
    void (*callback)(SceneControllerTask *), int group, int priority);
extern int func_ov005_02066920(
    void *archive_manager, int archive_slot, int member_id,
    void *destination, int argument_4, int argument_5);
extern int func_ov005_02067424(void *archive_manager, int archive_slot);
extern void func_ov007_0208a480(void);
extern void SceneTransitionController_Update(SceneControllerTask *task);

/* Metrowerks emits C functions in reverse source order. */
SceneControllerManagerLayout *SceneManager_Shutdown(
    SceneControllerManagerLayout *manager) {
    if (manager->controller_task != 0) {
        func_ov005_0206650c(manager->controller_task);
        manager->controller_task = 0;
    }
    SceneManager_ClearRuntime(manager);
    func_ov007_0208a480();

    if (manager->common_archives[5] != 0) {
        func_02029ba4(manager->common_archives[5]);
        manager->common_archives[5] = 0;
    }
    if (manager->common_archives[4] != 0) {
        func_02029ba4(manager->common_archives[4]);
        manager->common_archives[4] = 0;
    }
    if (manager->common_archives[3] != 0) {
        func_02029ba4(manager->common_archives[3]);
        manager->common_archives[3] = 0;
    }
    if (manager->common_archives[2] != 0) {
        func_02029ba4(manager->common_archives[2]);
        manager->common_archives[2] = 0;
    }
    if (manager->common_archives[1] != 0) {
        func_02029ba4(manager->common_archives[1]);
        manager->common_archives[1] = 0;
    }
    if (manager->common_archives[0] != 0) {
        func_02029ba4(manager->common_archives[0]);
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
