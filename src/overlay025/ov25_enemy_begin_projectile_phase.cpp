#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_BeginPause(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 9, -1);
        parameters->index = 30;
        task->update = Overlay25Enemy_UpdatePause;
    }
}

void Overlay25Enemy_UpdatePause(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    int timer = task->parameters.index;
    if (timer > 0)
        parameters->index = timer - 1;
    if (parameters->index <= 0) {
        BattleSceneObject_SetAnimation(object, 10, -1);
        task->update = Overlay25Enemy_BeginProjectilePhase;
    }
}

void Overlay25Enemy_BeginProjectilePhase(Overlay25Task *task, BattleSceneObject *object,
                                         Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 11, -1);
        parameters->index = 0;
        parameters->parameter = 0;
        task->update = func_ov025_020cb8d4;
    }
}
}
