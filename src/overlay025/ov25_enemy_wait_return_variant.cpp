#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_WaitProjectiles(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    for (int i = 1; i < 7; ++i)
        if (work->tasks[i].update)
            return;
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 12, -1);
        task->update = Overlay25Enemy_WaitReturnVariant;
    }
}

void Overlay25Enemy_WaitReturnVariant(Overlay25Task *task, BattleSceneObject *object,
                                      Overlay25WorkPrefix *work)
{
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        work->phase = 0;
        task->update = Overlay25Enemy_BeginReturn;
    }
}
}
