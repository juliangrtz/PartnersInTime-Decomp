#include "shop_scene_internal.h"

extern "C" {
GameIrqTask *ShopSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov009_0207e0a4;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *ShopSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov009_0207e0a4;
    GameIrqTask_DestroyBase(task);
    return task;
}
}
