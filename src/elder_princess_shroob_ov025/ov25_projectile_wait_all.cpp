/*
 * Elder Princess Shroob: projectile wait (overlay 25, 0x020C4F60-0x020C4FD8).
 *
 * Holds the sequence until every projectile has finished.
 */

#include "effect_task_internal.h"

extern "C" {
void Overlay25Projectile_WaitAll(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    int i;
    for (i = 1; i < 8; ++i)
        if (work->tasks[i].update)
            return;
    for (i = 0; i < 7; ++i)
        work->slots[i] = 0;
    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(42), -1, 1);
    task->update = 0;
}
}
