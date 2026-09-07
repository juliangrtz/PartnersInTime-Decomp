#include <game/window.h>
extern void *data_020565d4[];
extern void func_02020f4c(GameWindowManager *);
extern GameWindowManager *GameWindow_DestroyBase(GameWindowManager *);
GameWindowManager *GameSpriteWindow_Destroy(GameWindowManager *manager) {
    manager->vtable = data_020565d4;
    GameIrqTask_Disable(manager->irq_task);
    func_02020f4c(manager);
    GameWindow_DestroyBase(manager);
    return manager;
}
GameWindowManager *GameSpriteWindow_Delete(GameWindowManager *manager) {
    manager->vtable = data_020565d4;
    GameIrqTask_Disable(manager->irq_task);
    func_02020f4c(manager);
    GameWindow_DestroyBase(manager);
    GameHeap_Delete(manager);
    return manager;
}
