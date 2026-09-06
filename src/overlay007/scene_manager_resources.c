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
extern void *func_ov005_020670b4(
    void *archive_manager, u8 archive_slot, int member_id,
    void *size_output, int allocate_from_end);
extern void func_ov007_0208a5cc(void);
extern void SceneManager_InitializeRuntime(void *manager);

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
        manager->common_archives[archive_index] = func_ov005_020670b4(
            *(void **)(data_ov005_0206a180 + 0x2C),
            archive_index + 6, 0, 0, 1);
    }
    manager->controller_task = 0;
    MI_CpuFill8(
        manager->task_slots, 0, sizeof(manager->task_slots));
    func_ov007_0208a5cc();
    SceneManager_InitializeRuntime(manager);
    return manager;
}
