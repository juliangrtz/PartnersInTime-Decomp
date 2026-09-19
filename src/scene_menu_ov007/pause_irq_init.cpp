/*
 * Pause IRQ setup (overlay 7, 0x02073404-0x02073428).
 *
 * Creates the pause scene's VBlank task.
 */

#include "pause_scene_internal.h"

extern "C" {

GameIrqTask *PauseSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov007_0208d9a4;
    return task;
}
}
