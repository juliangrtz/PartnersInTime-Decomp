#include "effect_task_internal.h"
#include <game/battle_impact_effect.h>

extern "C" {
#include <game/random.h>
void func_ov025_020c9be0(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);

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
        task->update = Overlay25Enemy_LaunchLinkedEffect;
    }
}
void Overlay25Enemy_LaunchLinkedEffect(Overlay25Task *task, BattleSceneObject *enemy,
                                       Overlay25WorkPrefix *work)
{
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    if (!work->model_effect) {
        BattleSceneObject_SetAnimation(enemy, 15, -1);
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, linked);
        int target_z = 80 - position.y;
        int dz = 224 - target_z;
        int dx = 160 - linked->x;
        *(vu16 *)0x40002B0 = 0;
        *(vu32 *)0x40002B8 = dx * dx + dz * dz;
        while (*(vu16 *)0x40002B0 & 0x8000) {
        }
        int distance = *(vu32 *)0x40002B4;
        BattleSceneObject_StartMotionWithPeakDistance(linked, 2, dx, 0, dz, distance, distance, 2560, 1);
        BattleImpactEmitter_Start(40, 0, 4096, 3, 1, 16, 16, 64, 64, 0);
        BattleImpactEmitter_Start(41, 0, 4096, 3, 1, 8, 8, 64, 64, 0);
        BattleSpriteEffect_Spawn(512, position.x, position.y, position.z, 256);
        BattlePosition origin;
        Overlay25Object_GetViewPosition(&origin, enemy);
        BattleModelEffect_Spawn(804, enemy, (s16)(position.x - origin.x), (s16)(position.y - origin.y),
                                (s16)(position.z - origin.z), 256);
        task->update = Overlay25Enemy_BeginNextAnimation;
        BattleSound_Play(122, 0, 0, 0);
    }
}

void Overlay25Enemy_BeginNextAnimation(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *)
{
    BattleSceneObject_GetById(40);
    if (!object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 16, -1);
        task->update = Overlay25Enemy_FeedLinkedEffect;
    }
}

void Overlay25Enemy_FeedLinkedEffect(Overlay25Task *task, BattleSceneObject *enemy, Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    BattleSceneObject *next = BattleSceneObject_GetById(42);
    if (!BattleSceneObject_IsAnimationChannelActive(linked, 2)) {
        linked->flags.bits.stop_impact_particles = 1;
        BattleSceneObject_GetById(41)->flags.bits.stop_impact_particles = 1;
        int first =
            (u32)Random_NextModulo(*(u32 *)(gBattleContext + 27108) + *(u32 *)(gBattleContext + 27112)) <
            *(u32 *)(gBattleContext + 27108);
        int x = enemy->x, y = enemy->y, z = enemy->z - enemy->property_103;
        int sprite_id, model_id, start_x, start_y, start_z;
        if (first) {
            sprite_id = 516;
            start_x = x - 64;
            start_y = y + 1;
            start_z = z + 112;
            model_id = 810;
        } else {
            start_x = x + 64;
            sprite_id = 517;
            start_y = y + 1;
            model_id = 811;
            start_z = z + 112;
        }
        parameters->angle = 1 - first;
        --OVERLAY25_PROJECTILE_COUNTS[1 - first];
        BattleSceneObject_SetAnimation(next, 7 - first, -1);
        BattleSceneObject_AdjustPosition(next, start_x - next->x, start_y - next->y, start_z - next->z);
        next->property_103 = -12;
        next->flags.bits.unk_20_23 = 0;
        int dx = linked->x - start_x;
        int dy = linked->y - start_y;
        int dz = next->property_103 + (linked->z - linked->property_103) - start_z;
        *(vu16 *)0x40002B0 = 0;
        *(vu32 *)0x40002B8 = dx * dx + dy * dy + dz * dz;
        while (*(vu16 *)0x40002B0 & 0x8000) {
        }
        int distance = *(vu32 *)0x40002B4;
        int peak_distance;
        if (parameters->index < 1)
            peak_distance = distance - 8;
        else if (parameters->index < 2)
            peak_distance = distance - 16;
        else if (parameters->index < 4)
            peak_distance = distance - 24;
        else if (parameters->index < 6)
            peak_distance = distance - 32;
        else if (parameters->index < 9)
            peak_distance = distance - 40;
        else
            peak_distance = distance - 48;
        BattleSceneObject_StartMotionWithPeakDistance(next, 2, dx, dy, dz, peak_distance, distance, 2560, 0);
        work->chains[parameters->angle].unknown_318 = 2048;
        parameters->timer = 8;
        BattleModelEffect_Spawn(809, next, 0, 0, 0, 256);
        BattlePosition position, origin;
        Overlay25Object_GetViewPosition(&position, next);
        Overlay25Object_GetViewPosition(&origin, enemy);
        BattleSpriteEffect_Spawn(sprite_id, position.x, position.y, position.z, 256);
        BattleModelEffect_Spawn(model_id, enemy, (s16)(position.x - origin.x), (s16)(position.y - origin.y),
                                (s16)(position.z - origin.z), 256);
        BattleImpactEmitter_Start(42, 0, 4096, 0, 1, 16, 16, 64, 64, 0);
        BattleImpactEmitter_Start(43, 0, 4096, 0, 1, 8, 8, 64, 64, 0);
        task->update = Overlay25Enemy_UpdateLinkedEffectGrowth;
        BattleSound_Play(123, 0, 0, 0);
    }
}

void Overlay25Enemy_UpdateLinkedEffectGrowth(Overlay25Task *task, BattleSceneObject *enemy,
                                             Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    BattleSceneObject *linked = BattleSceneObject_GetById(40);
    BattleSceneObject *next = BattleSceneObject_GetById(42);
    BattleSceneObject *target = BattleSceneObject_GetById((u16)parameters->parameter);
    if (parameters->timer > 0)
        --parameters->timer;
    if (parameters->timer > 0)
        work->chains[parameters->angle].counter = (u16)work->chains[parameters->angle].counter + 4096;
    else
        work->chains[parameters->angle].unknown_318 = 416;
    if (!BattleSceneObject_IsAnimationChannelActive(next, 2)) {
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, BattleSceneObject_GetById(43));
        int effect_id;
        if (next->primary_model->animation_id == 6)
            effect_id = 812;
        else
            effect_id = 813;
        func_ov002_0206c148(effect_id, 0, position.x, position.y, position.z, 256);
        BattleSound_Play(270, 0, 0, 0);
        BattleSceneObject_SetAnimation(next, -1, -1);
        int animation = 0;
        int stage = ++parameters->index;
        int offset;
        s8 flags;
        if (stage == 1) {
            flags = 0;
            animation = 1;
            offset = -20;
        }
        if (stage == 2) {
            animation = 2;
            offset = -28;
            flags = 0;
        }
        if (stage == 4) {
            animation = 3;
            offset = -36;
            flags = 0;
        }
        if (stage == 6) {
            animation = 4;
            offset = -44;
            flags = 0;
        }
        if (stage == 9) {
            animation = 5;
            offset = -52;
            flags = 0;
        }
        if (animation) {
            BattleSceneObject_SetAnimation(linked, animation, -1);
            BattleSceneObject_AdjustPosition(linked, 0, 0, offset - linked->property_103);
            linked->property_103 = offset;
            linked->flags.bits.unk_20_23 = flags;
        }
        if (*(u32 *)(gBattleContext + 27108) || *(u32 *)(gBattleContext + 27112)) {
            task->update = Overlay25Enemy_FeedLinkedEffect;
        } else {
            int dx = target->x + target->property_0fc - linked->x;
            int dy = target->y + target->property_0fd - linked->y;
            int dz = target->z + target->property_0f8 - linked->z;
            *(vu16 *)0x40002B0 = 0;
            *(vu32 *)0x40002B8 = dx * dx + dy * dy + dz * dz;
            while (*(vu16 *)0x40002B0 & 0x8000) {
            }
            parameters->timer =
                BattleSceneObject_StartScaledAcceleratedMotion(
                    linked, 2, dx, dy, dz, *(vu32 *)0x40002B4 - linked->property_103, 0, 24, 1) /
                2;
            BattleSceneObject_SetAnimation(enemy, 17, -1);
            work->tasks[1].update = Overlay25Enemy_ResetStoppedAnimation;
            BattleImpactEmitter_Start(40, 0, 4096, 3, 1, 16, 16, 64, 64, 0);
            BattleImpactEmitter_Start(41, 0, 4096, 3, 1, 8, 8, 64, 64, 0);
            BattleActor_GetEnemySlot(enemy->actor_id)->damage_scale_q8 =
                384 * (10 * (parameters->index - 6) + 100) / 100;
            task->update = func_ov025_020c9be0;
            BattleSound_Play(35, 20, 0, 0);
        }
    }
}
}
