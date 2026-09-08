#include <game/heap.h>
#include <game/task.h>
#include <game/save_menu.h>

extern GameTask *func_ov006_02075b60(void *memory, u32 priority, void *argument);
extern GameTask *func_ov008_02077b1c(void *memory, u32 priority, void *argument);
extern GameTask *func_ov007_02072a80(void *memory, u32 priority, void *argument);
extern GameTask *func_ov009_0206f440(void *memory, u32 priority, void *argument);
extern GameTask *func_ov006_0207ab08(void *memory, u32 priority, void *argument);

GameTask *GameTask_CreateTitleScreen(void)
{
    void *memory = GameHeap_New(48, 0, 0, 0);
    if (memory) return func_ov006_02075b60(memory, 8, 0);
    return memory;
}

GameTask *func_020290a8(void)
{
    void *memory = GameHeap_New(48, 0, 0, 0);
    if (memory) return func_ov008_02077b1c(memory, 8, 0);
    return memory;
}

GameTask *GameTask_CreateSceneController(void)
{
    void *memory = GameHeap_New(56, 0, 0, 0);
    if (memory) return func_ov007_02072a80(memory, 8, 0);
    return memory;
}

GameTask *GameTask_CreateSaveMenu(void)
{
    void *memory = GameHeap_New(48, 0, 0, 0);
    if (memory) return (GameTask *)SaveScene_Init(memory, 8, 0);
    return memory;
}

GameTask *GameTask_CreateLoadMenu(void)
{
    void *memory = GameHeap_New(48, 0, 0, 0);
    if (memory) return (GameTask *)LoadScene_Init(memory, 8, 0);
    return memory;
}

GameTask *GameTask_CreateGameOver(void)
{
    void *memory = GameHeap_New(48, 0, 0, 0);
    if (memory) return (GameTask *)GameOverScene_Init(memory, 8, 0);
    return memory;
}

GameTask *GameTask_CreateShopMenu(void)
{
    void *memory = GameHeap_New(48, 0, 0, 0);
    if (memory) return func_ov009_0206f440(memory, 8, 0);
    return memory;
}

GameTask *GameTask_CreateStaffCredits(void)
{
    void *memory = GameHeap_New(4200, 0, 0, 0);
    if (memory) return func_ov006_0207ab08(memory, 8, 0);
    return memory;
}
