#include "effect_task_internal.h"

extern "C" {
void Overlay25Task_WaitSibling(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    if (!work->tasks[(task->parameters.index & 1) + 5].update)
        task->update = 0;
}
}
