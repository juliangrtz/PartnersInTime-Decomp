#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_damage_reaction.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>

enum BattleDamageReactionConstant {
    BATTLE_DAMAGE_REACTION_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_DAMAGE_REACTION_SCALE = 256,
    BATTLE_DAMAGE_REACTION_LAUNCH_CHANNEL = 2,
    BATTLE_DAMAGE_REACTION_RETURN_CHANNEL = 3,
    BATTLE_DAMAGE_REACTION_LAUNCH_COUNT = 3,
    BATTLE_DAMAGE_REACTION_LAUNCH_PHASE = 1,
    BATTLE_DAMAGE_REACTION_LAUNCH_OFFSET = 224,
    BATTLE_DAMAGE_REACTION_LAUNCH_SPEED = -128,
    BATTLE_DAMAGE_REACTION_MARIO_X = 56,
    BATTLE_DAMAGE_REACTION_MARIO_Y = 112,
    BATTLE_DAMAGE_REACTION_LUIGI_X = 46,
    BATTLE_DAMAGE_REACTION_LUIGI_Y = 152
};

enum BattleDamageReactionResource {
    BATTLE_DAMAGE_REACTION_SPECIAL_RESOURCE_1 = (s32)0xC1000082,
    BATTLE_DAMAGE_REACTION_SPECIAL_RESOURCE_2 = (s32)0xC10000A4
};

extern const u16 data_ov002_020bfa3c[];

int BattleParty_StartKnockout(BattleSceneObject *object);
int BattleParty_SpawnLaunchImpact(BattleActor *actor);

void BattleDamage_StartEnemyReaction(BattleSceneObject *object, int effect_id) {
    BattleActor *actor = BattleActor_GetById(object->actor_id);
    BattleDamageReactionTask *task = actor->damage_reaction_task;
    BattleDamageReactionPayload *payload;

    if (task == 0) {
        task = (BattleDamageReactionTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_DAMAGE_REACTION_TASK_POOL_OFFSET),
            0);
        BattleTask_BindOwnerSlot((BattleAITask *)task,
                                 (BattleAITask **)&actor->damage_reaction_task);
        payload = &task->data;
    } else {
        payload = &task->data;
        if (payload->effect != 0) {
            payload->effect->owner_slot = 0;
        }
    }

    BattleModelEffect_SpawnAttached((BattleEffect **)&payload->effect,
                                    effect_id, object, 0, 0, 0,
                                    BATTLE_DAMAGE_REACTION_SCALE);
    payload->actor_id = object->actor_id;
    BattleActor_GetById(object->actor_id)->flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
    task->callback = BattleDamage_UpdateEnemyReaction;
}

void BattleDamage_UpdateEnemyReaction(BattleAITask *base_task) {
    BattleDamageReactionTask *task = (BattleDamageReactionTask *)base_task;
    BattleDamageReactionPayload *payload = &task->data;
    BattleEnemyActor *enemy =
        (BattleEnemyActor *)BattleActor_GetEnemySlot(payload->actor_id);
    BattleSceneObject *object = enemy->actor.scene_object;
    BattleModel *model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));

    if (model != 0 && model->flag_bits.animation_active &&
        payload->effect != 0) {
        return;
    }

    task->callback = 0;
    if (model != 0 && model->flag_bits.animation_active &&
        !enemy->state_flag_bits.flag_04) {
        s16 animation_id = object->previous_base_animation_id;

        if (animation_id < 0) {
            object->animation_variant_offset += animation_id;
            BattleSceneObject_SetAnimation(object, 0, -1);
            object->animation_variant_offset -= animation_id;
        } else {
            BattleSceneObject_SetAnimation(object, animation_id, -1);
        }

        if (object->actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
            object->actor_id <
                BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT &&
            object->resource->resource_id !=
                BATTLE_DAMAGE_REACTION_SPECIAL_RESOURCE_1 &&
            object->resource->resource_id !=
                BATTLE_DAMAGE_REACTION_SPECIAL_RESOURCE_2) {
            enemy =
                (BattleEnemyActor *)BattleActor_GetEnemySlot(object->actor_id);
            object->property_102 += enemy->reaction_state.impact_offset_x;
            object->property_103 += enemy->reaction_state.impact_offset_y;
            enemy->reaction_state.impact_offset_x = 0;
            enemy->reaction_state.impact_offset_y = 0;
        }
    }

    BattleActor_GetById(payload->actor_id)->flags &=
        ~BATTLE_ACTOR_FLAG_HIT_LOCK;
}

void BattleDamage_StartEffectReaction(BattleSceneObject *object,
                                      int effect_id) {
    BattleActor *actor = BattleActor_GetById(object->actor_id);
    BattleDamageReactionTask *task = actor->damage_reaction_task;
    BattleDamageReactionPayload *payload;

    if (task == 0) {
        task = (BattleDamageReactionTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_DAMAGE_REACTION_TASK_POOL_OFFSET),
            0);
        BattleTask_BindOwnerSlot((BattleAITask *)task,
                                 (BattleAITask **)&actor->damage_reaction_task);
        payload = &task->data;
    } else {
        payload = &task->data;
        if (payload->effect != 0) {
            payload->effect->owner_slot = 0;
        }
    }

    BattleModelEffect_SpawnAttached((BattleEffect **)&payload->effect,
                                    effect_id, object, 0, 0, 0,
                                    BATTLE_DAMAGE_REACTION_SCALE);
    payload->actor_id = object->actor_id;
    BattleActor_GetById(object->actor_id)->flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
    task->callback = BattleDamage_UpdateEffectReaction;
}

void BattleDamage_UpdateEffectReaction(BattleAITask *base_task) {
    BattleDamageReactionTask *task = (BattleDamageReactionTask *)base_task;
    BattleDamageReactionPayload *payload = &task->data;
    BattleModel *model;

    BattleSceneObject_GetById(payload->actor_id);
    model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));
    if (model != 0 && model->flag_bits.animation_active &&
        payload->effect != 0) {
        return;
    }

    task->callback = 0;
    BattleActor_GetById(payload->actor_id)->flags &=
        ~BATTLE_ACTOR_FLAG_HIT_LOCK;
}

void BattleDamage_StartPartyReaction(BattleSceneObject *object) {
    BattleActor *actor = BattleActor_GetById(object->actor_id);
    BattleDamageReactionTask *task = actor->damage_reaction_task;

    if (task == 0) {
        task = (BattleDamageReactionTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_DAMAGE_REACTION_TASK_POOL_OFFSET),
            0);
        BattleTask_BindOwnerSlot((BattleAITask *)task,
                                 (BattleAITask **)&actor->damage_reaction_task);
    }

    task->data.actor_id = object->actor_id;
    BattleActor_GetById(object->actor_id)->flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
    task->callback = BattleDamage_UpdatePartyReaction;
}

void BattleDamage_UpdatePartyReaction(BattleAITask *base_task) {
    BattleDamageReactionTask *task = (BattleDamageReactionTask *)base_task;
    BattleDamageReactionPayload *payload = &task->data;
    BattleSceneObject *object =
        BattleActor_GetById(payload->actor_id)->scene_object;
    BattleModel *model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));

    if (!model->flag_bits.panel_animation_trigger) {
        return;
    }

    task->callback = 0;
    BattleEntity_BindResource(object->actor_id,
                              object->actor_id != BATTLE_ACTOR_MARIO);
    if (BattleActor_GetById(object->actor_id)->current_hp <= 0) {
        BattleParty_StartKnockout(object);
        return;
    }

    BattleSceneObject_SetAnimation(object, 0, -1);
    BattleActor_GetById(payload->actor_id)->flags &=
        ~BATTLE_ACTOR_FLAG_HIT_LOCK;
}

void BattleDamage_StartPartyLaunchReaction(BattleActor *actor) {
    BattleDamageReactionTask *task = actor->damage_reaction_task;
    BattleDamageReactionPayload *payload;

    if (task == 0) {
        task = (BattleDamageReactionTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_DAMAGE_REACTION_TASK_POOL_OFFSET),
            0);
        BattleTask_BindOwnerSlot((BattleAITask *)task,
                                 (BattleAITask **)&actor->damage_reaction_task);
    }

    payload = &task->data;
    payload->actor_id = actor->scene_object->actor_id;
    payload->launch_count = BATTLE_DAMAGE_REACTION_LAUNCH_COUNT;
    payload->launch_phase = BATTLE_DAMAGE_REACTION_LAUNCH_PHASE;
    actor->flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
    task->callback = BattleDamage_UpdatePartyLaunchReaction;
}

void BattleDamage_UpdatePartyLaunchReaction(BattleAITask *base_task) {
    BattleDamageReactionTask *task = (BattleDamageReactionTask *)base_task;
    BattleDamageReactionPayload *payload = &task->data;
    BattleSceneObject *object = BattleSceneObject_GetById(payload->actor_id);
    BattlePosition position;
    int launch_phase;

    BattlePosition_StoreViewRelative(
        &position, object->x, (s16)(object->y - object->z),
        (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);

    launch_phase = payload->launch_phase;
    if (launch_phase == 1) {
        if (position.y < 0) {
            BattleModel *model = BattleSceneObject_GetActiveModel(
                BattleSceneObject_GetById(payload->actor_id));
            BattleSceneObject_SetAnimation(object, model->animation_id, 1);
            object->motion_target_z -= BATTLE_DAMAGE_REACTION_LAUNCH_OFFSET;
            payload->launch_phase = 2;
        }
    } else if (launch_phase == 2) {
        if (position.y >= BATTLE_DAMAGE_REACTION_LAUNCH_OFFSET) {
            BattleModel *model = BattleSceneObject_GetActiveModel(
                BattleSceneObject_GetById(payload->actor_id));
            BattleSceneObject_SetAnimation(object, model->animation_id, 0);
            object->motion_target_z += BATTLE_DAMAGE_REACTION_LAUNCH_OFFSET;
            payload->launch_phase = 1;
        }
    }

    if (payload->launch_count != BATTLE_DAMAGE_REACTION_LAUNCH_COUNT) {
        if (BattleSceneObject_IsAnimationActiveById(
                payload->actor_id, BATTLE_DAMAGE_REACTION_LAUNCH_CHANNEL)) {
            return;
        }
        BattleParty_SpawnLaunchImpact(
            BattleActor_GetPartySlot(payload->actor_id));
    }

    if (payload->launch_count == 0) {
        task->callback = 0;
        BattleEntity_BindResource(object->actor_id,
                                  object->actor_id != BATTLE_ACTOR_MARIO);
        if (BattleActor_GetById(object->actor_id)->current_hp <= 0) {
            BattleParty_StartKnockout(object);
            return;
        }
        BattleSceneObject_SetAnimation(object, 0, -1);
        BattleActor_GetById(payload->actor_id)->flags &=
            ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        return;
    }

    if (payload->launch_count == BATTLE_DAMAGE_REACTION_LAUNCH_COUNT) {
        int delta_x;
        int delta_y;
        int total_duration;
        int index;
        int initial_speed;
        int zero;
        int one;
        int launch_channel;
        int terminal_speed;

        if (object->actor_id == BATTLE_ACTOR_MARIO) {
            delta_x = BATTLE_DAMAGE_REACTION_MARIO_X - object->x;
            delta_y = BATTLE_DAMAGE_REACTION_MARIO_Y - object->y;
        } else {
            delta_x = BATTLE_DAMAGE_REACTION_LUIGI_X - object->x;
            delta_y = BATTLE_DAMAGE_REACTION_LUIGI_Y - object->y;
        }

        total_duration = 0;
        index = BATTLE_DAMAGE_REACTION_LAUNCH_CHANNEL;
        initial_speed = -object->z;
        launch_channel = index;
        zero = total_duration;
        one = 1;
        terminal_speed = BATTLE_DAMAGE_REACTION_LAUNCH_SPEED;
        do {
            total_duration += BattleMotion_StartBallistic(
                object, launch_channel, zero, zero, one, initial_speed,
                terminal_speed, data_ov002_020bfa3c[index], one);
            initial_speed = zero;
            --index;
        } while (index >= 0);

        BattleSceneObject_MoveBy(object, BATTLE_DAMAGE_REACTION_RETURN_CHANNEL,
                                 delta_x, delta_y, zero, total_duration);
    }

    {
        int zero = 0;
        int one = 1;

        --payload->launch_count;
        BattleMotion_StartBallistic(
            object, BATTLE_DAMAGE_REACTION_LAUNCH_CHANNEL, zero, zero, one,
            -object->z, BATTLE_DAMAGE_REACTION_LAUNCH_SPEED,
            data_ov002_020bfa3c[payload->launch_count], one);
    }
}
