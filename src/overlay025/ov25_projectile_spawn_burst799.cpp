#include "effect_task_internal.h"

extern "C" {
void Overlay25Projectile_HideAndSignal(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(parameters->parameter + 40));
    BattleSceneObject_GetById((u16)parameters->timer);
    BattleSceneObject_SetAnimation(object, -1, -1);
    OVERLAY25_PROJECTILE_COUNTS[2] = 1;
    task->update = 0;
}

void Overlay25Projectile_SpawnBurst798(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(parameters->parameter + 40));
    BattleSceneObject_GetById((u16)parameters->timer);
    BattleSound_Play(271, 0, 0, 0);
    BattlePosition pos;
    Overlay25Object_GetViewPosition(&pos, object);
    BattleEffect *effect = BattleModelEffect_Spawn(798, 0, pos.x, pos.y, pos.z, 256);
    effect->user_value = (effect->user_value & 0xffff0000) | 53;
    BattleHitDescriptor_DisableByActor(object->actor_id);
    BattleSceneObject_SetAnimation(object, -1, -1);
    task->update = 0;
}

void Overlay25Projectile_SpawnBurst799(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(parameters->parameter + 40));
    BattleSceneObject_GetById((u16)parameters->timer);
    BattleSound_Play(271, 0, 0, 0);
    BattlePosition pos;
    Overlay25Object_GetViewPosition(&pos, object);
    BattleEffect *effect = BattleModelEffect_Spawn(799, 0, pos.x, pos.y, pos.z, 256);
    effect->user_value = (effect->user_value & 0xffff0000) | 53;
    BattleHitDescriptor_DisableByActor(object->actor_id);
    BattleSceneObject_SetAnimation(object, -1, -1);
    task->update = 0;
}
}
