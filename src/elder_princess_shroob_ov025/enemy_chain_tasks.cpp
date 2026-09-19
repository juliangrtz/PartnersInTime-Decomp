/*
 * Elder Princess Shroob: enemy chain tasks (overlay 25, 0x020C93D0-0x020C9BE0).
 *
 * The queued tasks a chained enemy effect runs through: starting its model
 * effect, picking the targets it tracks, waiting for the pair, and applying
 * reflected and landing damage.
 */

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

#include <game/battle_context.h>

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
    task->update = Overlay25Enemy_SelectTrackingTargets;
}

void Overlay25Enemy_SelectTrackingTargets(Overlay25Task *task, BattleSceneObject *enemy, Overlay25WorkPrefix *work)
{
    if (!work->model_effect) {
        BattleSceneObject_SetAnimation(enemy, 19, -1);
        if (BattleActor_CanReceiveStatus(BattleActor_GetById(56)) &&
            BattleActor_CanReceiveStatus(BattleActor_GetById(57))) {
            Overlay25ChainTrackingParameters *first = &work->tasks[3].chain_tracking;
            first->target_id = 56;
            first->index = 3;
            first->angle = 0;
            first->contact_count = OVERLAY25_PROJECTILE_COUNTS[0];
            work->chains[first->index].object_id = 40;
            work->tasks[3].update = Overlay25Chain_BeginTracking;
            Overlay25ChainTrackingParameters *second = &work->tasks[2].chain_tracking;
            second->target_id = 57;
            second->index = 2;
            second->angle = 0;
            second->contact_count = OVERLAY25_PROJECTILE_COUNTS[0];
            work->chains[second->index].object_id = 41;
            work->tasks[2].update = Overlay25Chain_BeginTracking;
        } else if (BattleActor_CanReceiveStatus(BattleActor_GetById(56))) {
            int index = ((BattleRuntimeFlags *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET))->bits.alternate_formation ? 2 : 3;
            Overlay25ChainTrackingParameters *parameters = &work->tasks[index].chain_tracking;
            parameters->target_id = 56;
            parameters->index = index;
            parameters->angle = 0;
            parameters->contact_count = OVERLAY25_PROJECTILE_COUNTS[0];
            work->chains[parameters->index].object_id = 40;
            work->tasks[index].update = Overlay25Chain_BeginTracking;
        } else {
            int index = ((BattleRuntimeFlags *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET))->bits.alternate_formation ? 3 : 2;
            Overlay25ChainTrackingParameters *parameters = &work->tasks[index].chain_tracking;
            parameters->target_id = 57;
            parameters->index = index;
            parameters->angle = 0;
            parameters->contact_count = OVERLAY25_PROJECTILE_COUNTS[0];
            work->chains[parameters->index].object_id = 40;
            work->tasks[index].update = Overlay25Chain_BeginTracking;
        }
        task->update = Overlay25Enemy_WaitPair;
    }
}

void func_ov025_020c8bf0(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
void Overlay25Enemy_WaitPair(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    if (!work->tasks[2].update && !work->tasks[3].update &&
        object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 0, -1);
        task->update = 0;
    }
}
void Overlay25Chain_BeginTracking(Overlay25Task *task, BattleSceneObject *enemy, Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    Overlay25ChainState *chain = &work->chains[task->parameters.index];
    BattleSceneObject *object = BattleSceneObject_GetById(chain->object_id);
    BattleSceneObject_GetById((u16)parameters->parameter);
    chain->x_q8 = (enemy->x + chain->offset_x) << 8;
    chain->y_q8 = (enemy->y + chain->offset_y) << 8;
    chain->z_q8 = (enemy->z + chain->offset_z - enemy->property_103) << 8;
    chain->counter = 0;
    parameters->timer = 0;
    parameters->mode = 0;
    chain->unknown_302 = -1;
    BattleEntity_BindResource(chain->object_id, enemy->resource->object_data_id);
    BattleSceneObject_SetAnimation(object, 30, -1);
    BattleSceneObject_AdjustPosition(object, enemy->x - object->x, enemy->y - object->y,
                                     enemy->z - object->z);
    object->flags.bits.suppress_shadow = 1;
    BattleSceneObject_SnapshotPosition(object);
    BattleHitDescriptor_Configure(chain->object_id, 56, 0, enemy->actor_id, 63);
    task->update = func_ov025_020c8bf0;
    BattleSound_Play(265, 0, 0, 0);
}
}
