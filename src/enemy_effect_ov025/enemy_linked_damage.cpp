#include "effect_task_internal.h"

extern "C" {
#include <game/battle_damage.h>
#include <game/battle_status.h>
void Overlay25LinkedEffect_ApplyReflectedDamage(Overlay25Task *task, BattleSceneObject *enemy,
                                                Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    if (!BattleSceneObject_IsAnimationChannelActive(linked, 2)) {
        BattleSound_Play(187, 0, 0, 0);
        BattleSceneObject_SetAnimation(linked, -1, -1);
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, linked);
        BattleSpriteEffect_Spawn(514, position.x, position.y, position.z, 256);
        BattleModelEffect_Spawn(806, 0, position.x, position.y, position.z, 256);
        BattleActor *actor = BattleActor_GetEnemySlot(enemy->actor_id);
        int damage = BattleDamage_ApplyEquipmentModifiers((u16)parameters->parameter, enemy->actor_id,
                                                          actor->damage_scale_q8, 1);
        BattleDamage_ApplyToEnemy(enemy, 0, 0, damage, 12, 7, 0);
        BattleSceneObject_GetById(40)->flags.bits.stop_impact_particles = 1;
        linked->flags.bits.stop_impact_particles = 1;
        BattleScreenEffect_StartPrimary(1, 120, 0);
        work->slots[0] = 0;
        work->slots[1] = 0;
        work->slots[2] = 0;
        task->update = 0;
    }
}
void Overlay25LinkedEffect_ApplyPartyLandingDamage(Overlay25Task *task, BattleSceneObject *enemy,
                                                   Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattlePartyActor *party = (BattlePartyActor *)BattleActor_GetPartySlot((u16)parameters->parameter);
    BattleSceneObject *object = party->actor.scene_object;
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, object);
        BattleSpriteEffect_Spawn(443, position.x, position.y, position.z, 256);
        BattleModelEffect_Spawn(646, 0, position.x, position.y, position.z, 256);
        int damage = BattleDamage_CalculateByObject(enemy->actor_id, (u16)parameters->parameter);
        BattleDamage_ApplyToParty(object, 0, 0, damage, 0, 0);
        BattleStatus_TryApply(BattleActor_GetById((u16)parameters->parameter), 2,
                              *(s16 *)(gBattleContext + 300), 10, 10);
        party->state_flags.raw &= ~0x1000;
        party->actor.flags &= ~0x8000;
        work->slots[0] = 0;
        work->slots[1] = 0;
        work->slots[2] = 0;
        task->update = 0;
    }
}
}
