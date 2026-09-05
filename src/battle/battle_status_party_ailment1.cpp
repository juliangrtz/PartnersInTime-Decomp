#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

enum BattleStatusPartyAilment1Constant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_STATUS_AILMENT1_ENTRY_ANIMATION = 24,
    BATTLE_STATUS_AILMENT1_HOLD_ANIMATION = 25,
    BATTLE_STATUS_AILMENT1_ADULT_DELAYED_EFFECT = 16,
    BATTLE_STATUS_AILMENT1_BABY_DELAYED_EFFECT = 17
};

enum BattleStatusPartyAilment1Sound {
    BATTLE_STATUS_AILMENT1_START_MARIO = 0x401C,
    BATTLE_STATUS_AILMENT1_START_LUIGI = 0x4045,
    BATTLE_STATUS_AILMENT1_START_BABY_MARIO = 0x4064,
    BATTLE_STATUS_AILMENT1_START_BABY_LUIGI = 0x4073
};

typedef struct BattleStatusPartyAilment1Task
    BattleStatusPartyAilment1Task;

typedef union BattleStatusPartyAilment1Flags {
    u16 raw;
    struct {
        u16 stop_after_animation : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusPartyAilment1Flags;

typedef struct BattleStatusPartyAilment1Payload {
    u16 actor_id;
    BattleStatusPartyAilment1Flags flags;
} BattleStatusPartyAilment1Payload;

struct BattleStatusPartyAilment1Task {
    BattleStatusPartyAilment1Task *next;
    u32 (*callback)(BattleStatusPartyAilment1Task *task);
    BattleStatusPartyAilment1Task **owner_slot;
    BattleStatusPartyAilment1Payload data;
};

typedef char BattleStatusPartyAilment1Task_SizeCheck[
    sizeof(BattleStatusPartyAilment1Task) == 0x10 ? 1 : -1
];

BattleAITask *BattleStatus_StartPartyAilment1Visual(int actor_id) {
    BattleStatusPartyAilment1Task *task =
        (BattleStatusPartyAilment1Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusPartyAilment1Payload *payload = &task->data;

    BattleActor_GetById(actor_id);
    payload->flags.bits.stop_after_animation = 0;
    payload->actor_id = actor_id;
    task->callback =
        (u32 (*)(BattleStatusPartyAilment1Task *))
            BattleStatus_BeginPartyAilment1Visual;
    return (BattleAITask *)task;
}

u32 BattleStatus_BeginPartyAilment1Visual(BattleAITask *base_task) {
    BattleStatusPartyAilment1Task *task =
        (BattleStatusPartyAilment1Task *)base_task;
    BattleStatusPartyAilment1Payload *payload = &task->data;
    BattlePartyActor *actor;
    u32 result;

    BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));
    actor = (BattlePartyActor *)BattleActor_GetPartySlot(payload->actor_id);
    if (task !=
        (BattleStatusPartyAilment1Task *)actor->actor.ailment_group_1.effect) {
        task->callback = 0;
        return 0;
    }

    result = actor->actor.flag_bits.hit_locked;
    if (!result) {
        switch (actor->formation_index) {
        case BATTLE_FORMATION_MARIO:
        case BATTLE_FORMATION_MARIO_CARRYING:
            BattleSound_Play(BATTLE_STATUS_AILMENT1_START_MARIO, 0, 0, 0);
            break;
        case BATTLE_FORMATION_LUIGI:
        case BATTLE_FORMATION_LUIGI_CARRYING:
            BattleSound_Play(BATTLE_STATUS_AILMENT1_START_LUIGI, 0, 0, 0);
            break;
        case BATTLE_FORMATION_BABY_MARIO:
            BattleSound_Play(
                BATTLE_STATUS_AILMENT1_START_BABY_MARIO, 0, 0, 0);
            break;
        case BATTLE_FORMATION_BABY_LUIGI:
            BattleSound_Play(
                BATTLE_STATUS_AILMENT1_START_BABY_LUIGI, 0, 0, 0);
            break;
        }

        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(payload->actor_id),
            BATTLE_STATUS_AILMENT1_ENTRY_ANIMATION, -1);
        int delayed_effect_id =
            actor->formation_index == BATTLE_FORMATION_BABY_MARIO ||
            actor->formation_index == BATTLE_FORMATION_BABY_LUIGI
                ? BATTLE_STATUS_AILMENT1_BABY_DELAYED_EFFECT
                : BATTLE_STATUS_AILMENT1_ADULT_DELAYED_EFFECT;
        BattleObjectEffect_SpawnDelayed(
            delayed_effect_id, BATTLE_STATUS_AILMENT1_ENTRY_ANIMATION,
            actor->actor.scene_object);
        u32 (*next_callback)(BattleStatusPartyAilment1Task *) =
            (u32 (*)(BattleStatusPartyAilment1Task *))
                BattleStatus_UpdatePartyAilment1Visual;
        actor->actor.flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
        task->callback = next_callback;
        return (u32)next_callback;
    }
    return result;
}

u32 BattleStatus_UpdatePartyAilment1Visual(BattleAITask *base_task) {
    BattleStatusPartyAilment1Task *task =
        (BattleStatusPartyAilment1Task *)base_task;
    BattleStatusPartyAilment1Payload *payload = &task->data;
    BattleModel *active_model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));
    BattleActor *actor = BattleActor_GetPartySlot(payload->actor_id);
    BattleStatusState *status = &actor->ailment_group_1;
    u32 stop_after_animation = payload->flags.bits.stop_after_animation;
    u32 result = (u32)actor;

    if (!stop_after_animation &&
        task != (BattleStatusPartyAilment1Task *)status->effect) {
        if (active_model->animation_id !=
            BATTLE_STATUS_AILMENT1_HOLD_ANIMATION) {
            task->callback = 0;
            return 0;
        }
        if (status->effect != 0) {
            task->callback = 0;
            return 0;
        }
        BattleSceneObject_SetAnimation(actor->scene_object, 0, -1);
        task->callback = 0;
        return 0;
    }

    if (!active_model->flag_bits.panel_animation_trigger) {
        return result;
    }

    if (stop_after_animation) {
        actor->flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        BattleSceneObject_SetAnimation(actor->scene_object, 0, -1);
        task->callback = 0;
        return 0;
    }

    if (active_model->animation_id ==
        BATTLE_STATUS_AILMENT1_ENTRY_ANIMATION) {
        actor->flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        BattleSceneObject_SetAnimation(
            actor->scene_object,
            BATTLE_STATUS_AILMENT1_HOLD_ANIMATION, -1);
        result = active_model->flags &
                 ~BATTLE_MODEL_ANIMATION_MODE_MASK;
        active_model->flags = result;
    }
    return result;
}
