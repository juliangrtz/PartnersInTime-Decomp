#include "pause_scene_internal.h"

extern "C" {

GameIrqTask *PauseSceneIrq_Delete(GameIrqTask *task)
{
    task->vtable = &data_ov007_0208d9a4;
    GameIrqTask_DestroyBase(task);
    GameHeap_Delete(task);
    return task;
}

GameIrqTask *PauseSceneIrq_Destroy(GameIrqTask *task)
{
    task->vtable = &data_ov007_0208d9a4;
    GameIrqTask_DestroyBase(task);
    return task;
}
}
