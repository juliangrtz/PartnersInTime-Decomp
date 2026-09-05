#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

enum BattleStatusPartyVisualConstant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_ANIMATION_OBSERVER_OFFSET = 0x68C8,
    BATTLE_STATUS_TRANSITION_ANIMATION = 22,
    BATTLE_STATUS_ACTIVE_ANIMATION = 32
};

enum BattleStatusPartyVisualSound {
    BATTLE_STATUS_START_MARIO = 0x401E,
    BATTLE_STATUS_START_LUIGI = 0x4042,
    BATTLE_STATUS_START_BABY_MARIO = 0x4067,
    BATTLE_STATUS_START_BABY_LUIGI = 0x407D,
    BATTLE_STATUS_ACTIVE_MARIO = 0x8009,
    BATTLE_STATUS_ACTIVE_LUIGI = 0x8010,
    BATTLE_STATUS_ACTIVE_BABY_MARIO = 0x801A,
    BATTLE_STATUS_ACTIVE_BABY_LUIGI = 0x8020
};

typedef struct BattleStatusPartyVisualTask BattleStatusPartyVisualTask;

typedef union BattleStatusPartyVisualFlags {
    u16 raw;
    struct {
        u16 active_animation_started : 1;
        u16 animation_layer : 4;
        u16 unknown_05_15 : 11;
    } bits;
} BattleStatusPartyVisualFlags;

typedef struct BattleStatusPartyVisualPayload {
    u16 actor_id;
    BattleStatusPartyVisualFlags flags;
} BattleStatusPartyVisualPayload;

struct BattleStatusPartyVisualTask {
    BattleStatusPartyVisualTask *next;
    void (*callback)(BattleStatusPartyVisualTask *task);
    BattleStatusPartyVisualTask **owner_slot;
    BattleStatusPartyVisualPayload data;
};

typedef char BattleStatusPartyVisualTask_SizeCheck[
    sizeof(BattleStatusPartyVisualTask) == 0x10 ? 1 : -1
];

BattleAITask *BattleStatus_StartPartyAilment4Visual(int actor_id) {
    BattleStatusPartyVisualTask *task =
        (BattleStatusPartyVisualTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusPartyVisualPayload *payload = &task->data;
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    BattleModel *model;

    payload->flags.bits.active_animation_started = 0;
    task->data.actor_id = actor_id;
    if (!actor->actor.flag_bits.hit_locked) {
        BattleSceneObject_SetAnimation(actor->actor.scene_object,
                                       BATTLE_STATUS_TRANSITION_ANIMATION, -1);
    }

    model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(actor_id));
    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
        payload->flags.bits.animation_layer = 0;
        break;
    case BATTLE_FORMATION_LUIGI:
        payload->flags.bits.animation_layer = 1;
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        payload->flags.bits.animation_layer = 2;
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        payload->flags.bits.animation_layer = 3;
        break;
    case BATTLE_FORMATION_MARIO_CARRYING:
        payload->flags.bits.animation_layer = 0;
        break;
    case BATTLE_FORMATION_LUIGI_CARRYING:
        payload->flags.bits.animation_layer = 1;
        break;
    }
    model->configure_animation_layer(
        payload->flags.bits.animation_layer,
        (s16)payload->flags.bits.animation_layer, 1);
    (*(BattleModel **)(gBattleContext + BATTLE_ANIMATION_OBSERVER_OFFSET))
        ->configure_animation_layer(
            payload->flags.bits.animation_layer,
            (s16)payload->flags.bits.animation_layer, 1);
    model->flags &= ~BATTLE_MODEL_ANIMATION_MODE_MASK;
    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
    case BATTLE_FORMATION_MARIO_CARRYING:
        BattleSound_Play(BATTLE_STATUS_START_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_LUIGI:
    case BATTLE_FORMATION_LUIGI_CARRYING:
        BattleSound_Play(BATTLE_STATUS_START_LUIGI, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        BattleSound_Play(BATTLE_STATUS_START_BABY_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        BattleSound_Play(BATTLE_STATUS_START_BABY_LUIGI, 0, 0, 0);
        break;
    }
    task->callback =
        (void (*)(BattleStatusPartyVisualTask *))
            BattleStatus_UpdatePartyAilmentVisual;
    return (BattleAITask *)task;
}

void BattleStatus_UpdatePartyAilmentVisual(BattleAITask *base_task) {
    BattleStatusPartyVisualTask *task =
        (BattleStatusPartyVisualTask *)base_task;
    BattleStatusPartyVisualPayload *payload = &task->data;
    BattleModel *model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(task->data.actor_id));
    BattleActor *actor = BattleActor_GetById(task->data.actor_id);
    BattleStatusState *status = &actor->ailment_group_2;

    if (!payload->flags.bits.active_animation_started &&
        task != (BattleStatusPartyVisualTask *)status->effect) {
        model->configure_animation_layer(
            payload->flags.bits.animation_layer, -1, 1);
        (*(BattleModel **)(gBattleContext + BATTLE_ANIMATION_OBSERVER_OFFSET))
            ->configure_animation_layer(
                payload->flags.bits.animation_layer, -1, 1);

        model->unknown_a0(model->unknown_a4());
        {
            int observer_animation_state =
                (*(BattleModel **)(gBattleContext +
                                   BATTLE_ANIMATION_OBSERVER_OFFSET))
                    ->unknown_a4();
            (*(BattleModel **)(gBattleContext +
                               BATTLE_ANIMATION_OBSERVER_OFFSET))
                ->unknown_a0(observer_animation_state);
        }

        if (model->animation_id != BATTLE_STATUS_TRANSITION_ANIMATION) {
            task->callback = 0;
        } else if (status->effect != 0) {
            task->callback = 0;
        } else {
            payload->flags.bits.active_animation_started = 1;
            actor->flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
            BattleSceneObject_SetAnimation(
                actor->scene_object, BATTLE_STATUS_ACTIVE_ANIMATION, -1);
            switch (((BattlePartyActor *)actor)->formation_index) {
            case BATTLE_FORMATION_MARIO:
            case BATTLE_FORMATION_MARIO_CARRYING:
                BattleSound_Play(BATTLE_STATUS_ACTIVE_MARIO, 50, 0, 0);
                break;
            case BATTLE_FORMATION_LUIGI:
            case BATTLE_FORMATION_LUIGI_CARRYING:
                BattleSound_Play(BATTLE_STATUS_ACTIVE_LUIGI, 50, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_MARIO:
                BattleSound_Play(BATTLE_STATUS_ACTIVE_BABY_MARIO, 50, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_LUIGI:
                BattleSound_Play(BATTLE_STATUS_ACTIVE_BABY_LUIGI, 50, 0, 0);
                break;
            }
        }

        if (task->callback == 0) {
            return;
        }
    }
    if (!model->flag_bits.panel_animation_trigger) {
        return;
    }
    if (!payload->flags.bits.active_animation_started) {
        return;
    }

    BattleSceneObject_SetAnimation(actor->scene_object, 0, -1);
    actor->flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
    task->callback = 0;
}

BattleAITask *BattleStatus_StartPartyAilment5Visual(int actor_id) {
    BattleStatusPartyVisualTask *task =
        (BattleStatusPartyVisualTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusPartyVisualPayload *payload = &task->data;
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    BattleModel *model;

    payload->flags.bits.active_animation_started = 0;
    task->data.actor_id = actor_id;
    if (!actor->actor.flag_bits.hit_locked) {
        BattleSceneObject_SetAnimation(actor->actor.scene_object,
                                       BATTLE_STATUS_TRANSITION_ANIMATION, -1);
    }

    model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(actor_id));
    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
        payload->flags.bits.animation_layer = 0;
        break;
    case BATTLE_FORMATION_LUIGI:
        payload->flags.bits.animation_layer = 1;
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        payload->flags.bits.animation_layer = 2;
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        payload->flags.bits.animation_layer = 3;
        break;
    case BATTLE_FORMATION_MARIO_CARRYING:
        payload->flags.bits.animation_layer = 0;
        break;
    case BATTLE_FORMATION_LUIGI_CARRYING:
        payload->flags.bits.animation_layer = 1;
        break;
    }
    model->configure_animation_layer(
        payload->flags.bits.animation_layer,
        (s16)(payload->flags.bits.animation_layer + 4), 1);
    (*(BattleModel **)(gBattleContext + BATTLE_ANIMATION_OBSERVER_OFFSET))
        ->configure_animation_layer(
            payload->flags.bits.animation_layer,
            (s16)(payload->flags.bits.animation_layer + 4), 1);
    model->flags &= ~BATTLE_MODEL_ANIMATION_MODE_MASK;
    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
    case BATTLE_FORMATION_MARIO_CARRYING:
        BattleSound_Play(BATTLE_STATUS_START_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_LUIGI:
    case BATTLE_FORMATION_LUIGI_CARRYING:
        BattleSound_Play(BATTLE_STATUS_START_LUIGI, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        BattleSound_Play(BATTLE_STATUS_START_BABY_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        BattleSound_Play(BATTLE_STATUS_START_BABY_LUIGI, 0, 0, 0);
        break;
    }
    task->callback =
        (void (*)(BattleStatusPartyVisualTask *))
            BattleStatus_UpdatePartyAilmentVisual;
    return (BattleAITask *)task;
}
