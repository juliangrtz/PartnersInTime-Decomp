#include "effect_task_internal.h"

extern "C" {
extern void func_0202cbd4(void *, int, u32);
void func_ov025_020c629c(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
void Overlay25Enemy_EmitTravelEffects(Overlay25Task *task, BattleSceneObject *object,
                                      Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    if (++parameters->index > 8) {
        parameters->index = 0;
        BattlePosition pos;
        Overlay25Object_GetViewPosition(&pos, object);
        BattleModelEffect_Spawn(796, 0, pos.x, pos.y, pos.z, 256);
    }
    if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
        if (parameters->travel_sound != -1) {
            func_ov002_02076178(parameters->travel_sound);
            parameters->travel_sound = -1;
        }
        if (object->primary_model->flag_bits.panel_animation_trigger) {
            BattleSound_Play(0, 0, 0, 0);
            BattleSceneObject_SetAnimation(object, 18, -1);
            BattlePosition pos;
            Overlay25Object_GetViewPosition(&pos, object);
            BattleSpriteEffect_Spawn(522, pos.x, pos.y, pos.z, 256);
            BattleModelEffect_SpawnAttached(&work->model_effect, 818, object, 0, 0, 0, 256);
            task->update = Overlay25Enemy_WaitAttachedEffect;
        }
    }
}

void Overlay25Enemy_WaitAttachedEffect(Overlay25Task *task, BattleSceneObject *object,
                                       Overlay25WorkPrefix *work)
{
    if (!work->model_effect) {
        BattleSceneObject_SetAnimation(object, 22, -1);
        task->update = Overlay25Enemy_InitializeSegmentChains;
    }
}
void Overlay25Enemy_InitializeSegmentChains(Overlay25Task *task, BattleSceneObject *enemy,
                                            Overlay25WorkPrefix *work)
{
    Overlay25ChainMotionParameters *parameters = &task->chain_motion;
    if (enemy->primary_model->flag_bits.panel_animation_trigger) {
        BattleSound_Play(265, 0, 0, 0);
        BattleScreenEffect_StartPrimary(3, 20, 0);
        BattleSceneObject_SetAnimation(enemy, 23, -1);
        BattlePosition position;
        Overlay25Object_GetViewPosition(&position, enemy);
        BattleSpriteEffect_Spawn(523, position.x, position.y, position.z, 256);
        BattleModelEffect_SpawnAttached(&work->model_effect, 819, enemy, 0, 0, 0, 256);
        func_0202cbd4(&work->chains[4], 0, 3344);
        u32 i = 0;
        if (i < OVERLAY25_PROJECTILE_COUNTS[0]) {
            Overlay25ChainState *chain = &work->chains[4];
            do {
                chain->unknown_304 = 4;
                chain->unknown_308 = 16;
                chain->countdown = 0;
                chain->unknown_31a = 16;
                chain->amplitude_q8 = 2048;
                BattleSceneObject *object = BattleSceneObject_GetById((u16)(i + 40));
                BattleEntity_BindResource(object->actor_id, enemy->resource->object_data_id);
                BattleSceneObject_SetAnimation(object, 30, -1);
                object->flags.bits.suppress_shadow = 1;
                BattleSceneObject_AdjustPosition(object, enemy->x - object->x, enemy->y - object->y,
                                                 enemy->z - object->z);
                BattleHitDescriptor_SetStatus(
                    BattleHitDescriptor_Configure(object->actor_id, 56, 0, enemy->actor_id, 63), 1, 4, 4);
                ++i;
                ++chain;
            } while (i < OVERLAY25_PROJECTILE_COUNTS[0]);
        }
        work->unknown1a2e = 1;
        parameters->phase_step = 0;
        parameters->phase = 0;
        parameters->timer = 0;
        parameters->angle = 0;
        parameters->flags[0] = 0;
        parameters->flags[1] = 0;
        parameters->flags[2] = 0;
        parameters->flags[3] = 0;
        task->update = func_ov025_020c629c;
    }
}
}
