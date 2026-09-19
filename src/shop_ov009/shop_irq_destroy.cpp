/*
 * Shop IRQ teardown (overlay 9, 0x0206AB80-0x0206ABD0).
 *
 * Destroy and Delete for the shop scene's VBlank task.
 */

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
