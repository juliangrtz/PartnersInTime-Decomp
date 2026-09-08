#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_WaitAnimation(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 0, -1);
        task->update = 0;
    }
}
}
