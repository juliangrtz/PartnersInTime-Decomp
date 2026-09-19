#include "shop_scene_internal.h"

extern "C" {
GameIrqTask *ShopSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov009_0207e0a4;
    return task;
}
}
