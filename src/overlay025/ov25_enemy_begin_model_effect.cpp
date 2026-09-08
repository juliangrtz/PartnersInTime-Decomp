#include "effect_task_internal.h"

extern "C" {
void Overlay25Enemy_BeginModelEffect(Overlay25Task *task, BattleSceneObject *object,
                                     Overlay25WorkPrefix *work)
{
    BattleGlobalProperty_Set(12, 1);
    BattleSceneObject_SetAnimation(object, 18, -1);
    BattlePosition pos;
    Overlay25Object_GetViewPosition(&pos, object);
    BattleSpriteEffect_Spawn(518, pos.x, pos.y, pos.z, 256);
    BattleModelEffect_SpawnAttached(&work->model_effect, 814, object, 0, 0, 0, 256);
    BattleActor_GetEnemySlot(object->actor_id)->damage_scale_q8 = 204;
    BattleSound_Play(0, 0, 0, 0);
    task->update = func_ov025_020c9580;
}
}
