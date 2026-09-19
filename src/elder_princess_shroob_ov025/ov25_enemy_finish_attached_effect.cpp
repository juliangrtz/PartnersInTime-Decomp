/*
 * Elder Princess Shroob: attached effect finish (overlay 25, 0x020C822C-0x020C8268).
 *
 * Finishes an effect that was attached to the enemy.
 */

#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_FinishAttachedEffect(Overlay25Task *task, BattleSceneObject *object,
                                         Overlay25WorkPrefix *work)
{
    if (!work->model_effect) {
        BattleSceneObject_SetAnimation(object, 20, -1);
        task->update = 0;
    }
}
}
