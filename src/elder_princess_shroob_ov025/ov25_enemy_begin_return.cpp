/*
 * Elder Princess Shroob: enemy return (overlay 25, 0x020C30CC-0x020C31A0).
 *
 * Begins the enemy's return to its position.
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
    task->update = func_ov025_020c2ee4;
}
}
