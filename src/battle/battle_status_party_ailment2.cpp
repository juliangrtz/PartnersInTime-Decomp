#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

enum BattleStatusPartyAilment2Constant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_STATUS_AILMENT2_TRANSITION_ANIMATION = 27,
    BATTLE_STATUS_AILMENT2_ACTIVE_ANIMATION = 28,
    BATTLE_STATUS_AILMENT2_MODEL_EFFECT = 741,
    BATTLE_STATUS_AILMENT2_MODEL_SCALE = 256
};

enum BattleStatusPartyAilment2Sound {
    BATTLE_STATUS_AILMENT2_START_MARIO = 0x401D,
    BATTLE_STATUS_AILMENT2_START_LUIGI = 0x4046,
    BATTLE_STATUS_AILMENT2_START_BABY_MARIO = 0x405F,
    BATTLE_STATUS_AILMENT2_START_BABY_LUIGI = 0x407A,
    BATTLE_STATUS_AILMENT2_ACTIVE_MARIO = 0x800D,
    BATTLE_STATUS_AILMENT2_ACTIVE_LUIGI = 0x8013,
    BATTLE_STATUS_AILMENT2_ACTIVE_BABY_MARIO = 0x8019,
    BATTLE_STATUS_AILMENT2_ACTIVE_BABY_LUIGI = 0x8021
};

typedef struct BattleStatusPartyAilment2Task
    BattleStatusPartyAilment2Task;

typedef union BattleStatusPartyAilment2TaskFlags {
    u16 raw;
    struct {
        u16 active_animation_started : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusPartyAilment2TaskFlags;

typedef union BattleStatusPartyAilment2EffectFlags {
    u16 raw;
    struct {
        u16 active : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusPartyAilment2EffectFlags;

typedef struct BattleStatusPartyAilment2Effect {
    u8 unknown_00[4];
    void (*update_callback)(struct BattleStatusPartyAilment2Effect *effect);
    u8 unknown_08[0x0E];
    s16 animation_offset_y;
    u8 unknown_18[2];
    BattleStatusPartyAilment2EffectFlags flags;
} BattleStatusPartyAilment2Effect;

typedef struct BattleStatusPartyAilment2Payload {
    BattleStatusPartyAilment2Effect *effect;
    u16 actor_id;
    s16 previous_object_offset_y;
    BattleStatusPartyAilment2TaskFlags flags;
} BattleStatusPartyAilment2Payload;

struct BattleStatusPartyAilment2Task {
    BattleStatusPartyAilment2Task *next;
    void (*callback)(BattleStatusPartyAilment2Task *task);
    BattleStatusPartyAilment2Task **owner_slot;
    BattleStatusPartyAilment2Payload data;
};

typedef char BattleStatusPartyAilment2Task_SizeCheck[
    sizeof(BattleStatusPartyAilment2Task) == 0x18 ? 1 : -1
];
typedef char BattleStatusPartyAilment2EffectSizeCheck[
    sizeof(BattleStatusPartyAilment2Effect) == 0x1C ? 1 : -1
];

BattleAITask *BattleStatus_StartPartyAilment2Visual(int actor_id) {
    BattleStatusPartyAilment2Task *task =
        (BattleStatusPartyAilment2Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusPartyAilment2Payload *payload = &task->data;
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetById(actor_id);
    BattleSceneObject *object = actor->actor.scene_object;

    payload->flags.bits.active_animation_started = 0;
    payload->actor_id = actor_id;
    payload->effect =
        (BattleStatusPartyAilment2Effect *)BattleModelEffect_Spawn(
            BATTLE_STATUS_AILMENT2_MODEL_EFFECT, object,
            object->property_0fc,
            (s16)(object->property_0fd - object->property_0f8 +
                  object->property_103),
            (s16)-object->property_0fd,
            BATTLE_STATUS_AILMENT2_MODEL_SCALE);
    payload->effect->flags.bits.active = 1;
    payload->previous_object_offset_y = object->property_103;

    if (!actor->actor.flag_bits.hit_locked) {
        BattleSceneObject_SetAnimation(
            actor->actor.scene_object,
            BATTLE_STATUS_AILMENT2_TRANSITION_ANIMATION, -1);
    }
    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
    case BATTLE_FORMATION_MARIO_CARRYING:
        BattleSound_Play(BATTLE_STATUS_AILMENT2_START_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_LUIGI:
    case BATTLE_FORMATION_LUIGI_CARRYING:
        BattleSound_Play(BATTLE_STATUS_AILMENT2_START_LUIGI, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        BattleSound_Play(BATTLE_STATUS_AILMENT2_START_BABY_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        BattleSound_Play(BATTLE_STATUS_AILMENT2_START_BABY_LUIGI, 0, 0, 0);
        break;
    }
    task->callback =
        (void (*)(BattleStatusPartyAilment2Task *))
            BattleStatus_UpdatePartyAilment2Visual;
    return (BattleAITask *)task;
}

u32 BattleStatus_UpdatePartyAilment2Visual(BattleAITask *base_task) {
    BattleStatusPartyAilment2Task *task =
        (BattleStatusPartyAilment2Task *)base_task;
    BattleStatusPartyAilment2Payload *payload = &task->data;
    BattleActor *actor;
    BattleSceneObject *object;
    BattleStatusState *status;
    BattleModel *active_model;
    u32 result;

    active_model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));
    actor = BattleActor_GetById(payload->actor_id);
    status = &actor->ailment_group_1;
    object = actor->scene_object;

    if (actor->status_visual_flag_bits.suppress_status_model) {
        if (payload->effect != 0) {
            payload->effect->update_callback = 0;
            payload->effect = 0;
        }
    } else if (payload->effect == 0) {
        payload->effect =
            (BattleStatusPartyAilment2Effect *)BattleModelEffect_Spawn(
                BATTLE_STATUS_AILMENT2_MODEL_EFFECT, object,
                object->property_0fc,
                (s16)(object->property_0fd - object->property_0f8 +
                      object->property_103),
                (s16)-object->property_0fd,
                BATTLE_STATUS_AILMENT2_MODEL_SCALE);
        payload->effect->flags.bits.active = 1;
        payload->previous_object_offset_y = object->property_103;
    }

    if (payload->effect != 0) {
        payload->effect->animation_offset_y +=
            object->property_103 - payload->previous_object_offset_y;
        payload->previous_object_offset_y = object->property_103;
    }

    u16 task_flags = payload->flags.raw;
    u32 active_animation_started =
        payload->flags.bits.active_animation_started;
    if (!active_animation_started &&
        task != (BattleStatusPartyAilment2Task *)status->effect) {
        if (active_model->animation_id !=
            BATTLE_STATUS_AILMENT2_TRANSITION_ANIMATION) {
            task->callback = 0;
        } else if (status->effect != 0) {
            task->callback = 0;
        } else {
            payload->flags.raw = task_flags & ~1 | 1;
            actor->flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
            BattleSceneObject_SetAnimation(
                object, BATTLE_STATUS_AILMENT2_ACTIVE_ANIMATION, -1);
            switch (((BattlePartyActor *)actor)->formation_index) {
            case BATTLE_FORMATION_MARIO:
            case BATTLE_FORMATION_MARIO_CARRYING:
                BattleSound_Play(
                    BATTLE_STATUS_AILMENT2_ACTIVE_MARIO, 0, 0, 0);
                break;
            case BATTLE_FORMATION_LUIGI:
            case BATTLE_FORMATION_LUIGI_CARRYING:
                BattleSound_Play(
                    BATTLE_STATUS_AILMENT2_ACTIVE_LUIGI, 0, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_MARIO:
                BattleSound_Play(
                    BATTLE_STATUS_AILMENT2_ACTIVE_BABY_MARIO, 0, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_LUIGI:
                BattleSound_Play(
                    BATTLE_STATUS_AILMENT2_ACTIVE_BABY_LUIGI, 0, 0, 0);
                break;
            }
        }

        result = (u32)task->callback;
        if (result != 0) {
            return result;
        }
        if (payload->effect != 0) {
            result = 0;
            payload->effect->update_callback = 0;
        }
        return result;
    }

    result = active_model->flag_bits.panel_animation_trigger;
    if (!result || !active_animation_started) {
        return result;
    }

    actor->flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
    BattleSceneObject_SetAnimation(object, 0, -1);
    if (payload->effect != 0) {
        payload->effect->update_callback = 0;
    }
    task->callback = 0;
    return 0;
}
