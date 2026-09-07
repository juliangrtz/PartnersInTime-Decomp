extern "C" {
#include <game/window.h>
extern GameTaskVTable data_020564d4;
}
class GameWindowUploadInterface {
public:
    virtual void destroy();
    virtual void delete_manager();
    virtual void update();
    virtual void upload();
};
extern "C" {
void GameWindowIrq_Update(GameIrqTask *task) {
    GameWindowManager *manager = (GameWindowManager *)task->argument;
    if (manager->state.bits.enabled) {
        GameWindow_ApplyScroll(manager);
        if (manager->state.bits.main_map_dirty) GameWindow_UploadMainTilemap(manager);
        if (manager->state.bits.sub_map_dirty) GameWindow_UploadSubTilemap(manager);
        if (manager->state.bits.dirty) ((GameWindowUploadInterface *)manager)->upload();
    }
}
GameIrqTask *GameWindowIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *argument) {
    GameIrqTask_Init(task, priority, unused, argument);
    task->vtable = &data_020564d4;
    return task;
}
GameIrqTask *GameWindowIrq_Destroy(GameIrqTask *task) {
    task->vtable = &data_020564d4;
    GameIrqTask_DestroyBase(task);
    return task;
}
GameIrqTask *GameWindowIrq_Delete(GameIrqTask *task) {
    task->vtable = &data_020564d4;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}
}
