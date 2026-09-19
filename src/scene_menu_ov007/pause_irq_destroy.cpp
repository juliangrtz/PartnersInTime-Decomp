/*
 * Pause IRQ teardown (overlay 7, 0x0206AB80-0x0206ABD0).
 *
 * Destroy and Delete for the pause scene's VBlank task.
 */

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
