#include "save_menu_internal.h"

GameIrqTask *EraseSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov008_02078250;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *EraseSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov008_02078250;
    GameIrqTask_DestroyBase(task);
    return task;
}
