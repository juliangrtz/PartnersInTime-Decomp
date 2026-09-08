#include "frontend_scene_internal.h"

GameIrqTask *CreditsSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207bba4;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *CreditsSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov006_0207bba4;
    GameIrqTask_DestroyBase(task);
    return task;
}
