#include "save_menu_internal.h"

GameIrqTask *LoadSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov008_02077f78;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *LoadSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov008_02077f78;
    GameIrqTask_DestroyBase(task);
    return task;
}
