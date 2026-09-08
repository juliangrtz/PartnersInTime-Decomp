#include "frontend_scene_internal.h"

GameIrqTask *TitleSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207b508;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *TitleSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207b508;
    GameIrqTask_DestroyBase(task);
    return task;
}
