#include "effect_task_internal.h"

extern "C" {
void func_ov025_020cad04(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);

void Overlay25Projectile_CheckHit(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *object = BattleSceneObject_GetById((u16)(parameters->parameter + 40));
    BattleSceneObject_GetById((u16)parameters->timer);
    for (BattleHitRecord *hit = (BattleHitRecord *)(gBattleContext + 51928);; ++hit) {
        if (!hit->kind)
            break;
        if (hit->source_id == object->actor_id) {
            task->update = Overlay25Projectile_SpawnBurst798;
            return;
        }
        if (hit->target_id == object->actor_id) {
            task->update = Overlay25Projectile_SpawnBurst799;
            return;
        }
    }
    if (!BattleSceneObject_IsAnimationChannelActive(object, 1)) {
        BattleSceneObject_SetAnimation(object, -1, -1);
        task->update = Overlay25Projectile_HideAndSignal;
    }
}

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

void Overlay25Enemy_BeginLinkedEffects(Overlay25Task *task, BattleSceneObject *, Overlay25WorkPrefix *)
{
    Overlay25Parameters *parameters = &task->parameters;
    parameters->mode_flags &= ~1;
    task->parameters.index = 0;
    if (!BattleActor_CanReceiveStatus(BattleActor_GetById(56)))
        parameters->parameter = 57;
    else if (!BattleActor_CanReceiveStatus(BattleActor_GetById(57)))
        parameters->parameter = 56;
    else if (*(u32 *)(gBattleContext + 27108) < *(u32 *)(gBattleContext + 27112))
        parameters->parameter = 57;
    else
        parameters->parameter = 56;
    BattleGlobalProperty_Set(12, 1);
    BattleObjectData_AllocateLoadBuffer(52, 16384);
    BattleObjectData_ConfigureLoad(52, 2432, 0);
    BattleObjectData_EnsureLoaded(52, 0xc1000098);
    task->update = Overlay25Enemy_InitializeLinkedEffects;
}

void Overlay25Enemy_InitializeLinkedEffects(Overlay25Task *task, BattleSceneObject *object,
                                            Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    if (!BattleObjectData_IsLoadPending(52)) {
        BattleEntity_BindResource(40, 52);
        work->slots[0] = 40;
        work->secondary_slots[0] = 41;
        work->slot_depth_offsets[0] = 32;
        BattleEntity_BindResource(42, 52);
        work->slots[1] = 42;
        work->secondary_slots[1] = 43;
        work->slot_depth_offsets[1] = 32;
        BattleSceneObject_SetAnimation(object, 13, -1);
        BattleSceneObject_AdjustPosition(linked, object->x - 20 - linked->x, object->y + 4 - linked->y,
                                         object->z + 37 - object->property_103 - linked->z);
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, linked);
        BattleSpriteEffect_Spawn(510, pos.x, pos.y, pos.z, 256);
        BattleModelEffect_Spawn(802, object, 0, 0, 0, 256);
        task->update = Overlay25Enemy_WaitLinkedEffectAnimation;
        BattleSound_Play(97, 0, 0, 0);
    }
}

void Overlay25Enemy_WaitLinkedEffectAnimation(Overlay25Task *task, BattleSceneObject *object,
                                              Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    if (!object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 14, -1);
        BattleSceneObject_SetAnimation(linked, 0, -1);
        linked->property_103 = -12;
        linked->flags.bits.unk_20_23 = 0;
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, linked);
        BattleSpriteEffect_Spawn(511, pos.x, pos.y, pos.z, 256);
        BattleModelEffect_SpawnAttached(&work->model_effect, 803, linked, 0, 0, 0, 256);
        task->update = func_ov025_020cad04;
    }
}
}
