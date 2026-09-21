/*
 * Elder Princess Shroob: enemy animation wait (overlay 25, 0x020C7210-0x020C7268).
 *
 * Holds the sequence until the enemy's animation has played out.
 */

#include "effect_task_internal.h"

extern "C" {
void Overlay25Task_WaitFirstChild(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *work)
{
    if (!work->tasks[1].update)
        task->update = 0;
}

void Overlay25Enemy_WaitAnimation(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    if (object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 0, -1);
        task->update = 0;
    }
}
}
