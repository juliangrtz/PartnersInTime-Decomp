/*
 * Game-over IRQ teardown (overlay 8, 0x0206FFD8-0x02070028).
 *
 * Destroy and Delete for the game-over scene's VBlank task.
 */

#include "save_menu_internal.h"

GameIrqTask *GameOverSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov008_02077fdc;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *GameOverSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov008_02077fdc;
    GameIrqTask_DestroyBase(task);
    return task;
}
