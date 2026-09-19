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
