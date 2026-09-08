#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_WaitReturn(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        work->phase = 0;
        task->update = Overlay25Enemy_BeginReturn;
    }
}
}
