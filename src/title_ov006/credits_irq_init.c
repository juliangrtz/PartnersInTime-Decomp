/*
 * Credits IRQ task setup (overlay 6, 0x0207AF14-0x0207AF38).
 *
 * Creates the credits scene's VBlank task.
 */

#include "frontend_scene_internal.h"

GameIrqTask *CreditsSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov006_0207bba4;
    return task;
}
