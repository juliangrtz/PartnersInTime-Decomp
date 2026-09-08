#include "frontend_scene_internal.h"

GameIrqTask *CreditsSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov006_0207bba4;
    return task;
}
