/*
 * Elder Princess Shroob: sibling task wait (overlay 25, 0x020C8AC8-0x020C8AF4).
 *
 * Holds a task until the sibling task it was started with finishes.
 */

#include "effect_task_internal.h"

extern "C" {
void Overlay25Task_WaitSibling(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    if (!work->tasks[(task->parameters.index & 1) + 5].update)
        task->update = 0;
}
}
