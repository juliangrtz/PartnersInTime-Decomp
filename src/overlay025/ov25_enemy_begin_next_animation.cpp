#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_BeginNextAnimation(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    BattleSceneObject_GetById(40);
    if (!object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 16, -1);
        task->update = func_ov025_020ca858;
    }
}
}
