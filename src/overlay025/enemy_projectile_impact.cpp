#include "effect_task_internal.h"

extern "C" {
#include <game/battle_damage.h>

void Overlay25Projectile_UpdateReflectedImpact(Overlay25Task *task, BattleSceneObject *enemy,
                                               Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(task->parameters.index + 44));
    if (++parameters->timer > 8) {
        parameters->timer = 0;
        BattlePosition trail;
        Overlay25Object_GetViewPosition(&trail, object);
        BattleModelEffect_Spawn(830, 0, trail.x, trail.y, trail.z, 256);
    }
    object->primary_model->rotation_z += 4096;
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
        BattlePosition impact;
        BattleSound_Play(272, 0, 0, 0);
        Overlay25Object_GetViewPosition(&impact, object);
        BattleSpriteEffect_Spawn(452, impact.x, impact.y, impact.z, 256);
        BattleModelEffect_Spawn(666, 0, impact.x, impact.y, impact.z, 256);
        if (parameters->mode != 2) {
            BattleDamage_ApplyToEnemy(
                enemy, 0, 0, BattleDamage_CalculateByObject((u16)parameters->parameter, enemy->actor_id), 12,
                7, 0);
        } else {
            BattleDamage_ApplyToEnemy(
                enemy, -32, 0, BattleDamage_CalculateByObject((u16)parameters->parameter, enemy->actor_id),
                12, 7, 0);
        }
        BattleSceneObject_SetAnimation(object, 14, -1);
        object->property_103 = 0;
        BattleSceneObject_AdjustPosition(object, 0, 0, 24);
        Overlay25Object_GetViewPosition(&impact, object);
        int duration = BattleMotion_StartBallistic(object, 2, 0, 0, -1, 216 - impact.y, 64, -32, 1);
        BattleSceneObject_MoveBy(object, 1, (duration * 256) / 256, 0, 0, duration);
        task->update = Overlay25Projectile_UpdateSpin;
    }
}

void Overlay25Projectile_UpdateSpin(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(task->parameters.index + 44));
    BattleHitDescriptor_DisableByActor(object->actor_id);
    object->primary_model->rotation_z += 4096;
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2))
        task->update = 0;
}

void Overlay25EffectSequence_Initialize(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleObjectData_AllocateLoadBuffer(52, 16384);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_GetLoadState(52)->flags.bits.allocated = 1;
    BattleObjectData_GetLoadState(52)->flags.raw &= ~0x08000000;
    BattleObjectData_EnsureLoaded(52, 0xc1000099);
    BattleObjectData_AllocateLoadBuffer(53, 16384);
    BattleObjectData_ConfigureLoad(53, 2532, 0);
    BattleObjectData_EnsureLoaded(53, 0xc100007e);
    BattleObjectData_AllocateLoadBuffer(54, 16384);
    BattleObjectData_ConfigureLoad(54, 2682, 0);
    BattleObjectData_EnsureLoaded(54, 0xc100009a);
    task->parameters.index = 0;
    parameters->parameter = 0;
    parameters->timer = 0;
    parameters->angle = 0;
    parameters->mode = 1;
    task->update = Overlay25EffectSequence_BeginPositioning;
}

void Overlay25EffectSequence_BeginPositioning(Overlay25Task *task, BattleSceneObject *object,
                                              Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (!BattleObjectData_IsLoadPending(54)) {
        BattleEntity_BindResource(43, 54);
        BattleSceneObject_SetAnimation(object, 7, -1);
        int dy = 128 - object->y, dx = 240 - object->x;
        Overlay25Object_StartDistance(dx, dy);
        BattleSceneObject_MoveBy(object, 2, dx, dy, 0, ((int)*(vu32 *)0x40002B4 << 8) / 256);
        parameters->sound = BattleSound_Play(24, 0, -1, 0);
        task->update = func_ov025_020c4084;
    }
}
}
