/*
 * Elder Princess Shroob: enemy return (overlay 25, 0x020C2EE4-0x020C31A0).
 *
 * Returns the enemy to its position, emits its trail and finishes the task.
 */

#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_BeginReturn(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    BattleSceneObject_SetAnimation(object, 7, -1);
    BattleActor *actor = BattleActor_GetEnemySlot(object->actor_id);
    int dx = actor->unk_018 - object->x;
    int dy = actor->unk_01a - object->y;
    Overlay25Object_StartDistance(dx, dy);
    BattleSceneObject_MoveBy(object, 2, dx, dy, 0, ((int)*(vu32 *)0x40002B4 << 8) / 256);
    task->sound_handle = BattleSound_Play(24, 0, -1, 0);
    task->update = Overlay25Enemy_UpdateReturn;
}
}

extern "C" void Overlay25Enemy_UpdateReturn(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    if (object->x < object->motion_origin_x) ++work->phase;
    if (work->phase > 8) {
        work->phase = 0;
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, object);
        BattleModelEffect_Spawn(796, 0, position.x, position.y, position.z, 256);
    }
    /* Re-read after the first effect call: callbacks can change the object. */
    if (object->motion_origin_x < object->x) ++work->phase;
    if (work->phase > 8) {
        work->phase = 0;
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, object);
        BattleModelEffect_Spawn(793, 0, position.x, position.y, position.z, 256);
    }
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
        if (task->sound_handle != -1) {
            BattleSound_Stop(task->sound_handle);
            task->sound_handle = -1;
        }
        if ((u32)(object->primary_model->flags << 29) >> 31) {
            BattleSceneObject_SetAnimation(object, 0, -1);
            task->update = 0;
        }
    }
}
