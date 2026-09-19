#include "save_menu_internal.h"

GameIrqTask *LoadSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov008_02077f78;
    return task;
}
