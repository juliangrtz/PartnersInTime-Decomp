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
