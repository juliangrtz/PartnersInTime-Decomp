/*
 * Save scene IRQ teardown (overlay 8, 0x0206AB80-0x0206ABD0).
 *
 * Destroy and Delete for the save scene's VBlank task.
 */

#include "save_menu_internal.h"

GameIrqTask *SaveSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov008_02077f10;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *SaveSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov008_02077f10;
    GameIrqTask_DestroyBase(task);
    return task;
}
