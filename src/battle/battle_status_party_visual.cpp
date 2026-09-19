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

enum BattleStatusPartyAilment2Constant {
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

#include <game/battle_object.h>

extern "C" {
void *GameHeap_New(u32 size, u32 heap_id, const void *allocator,
                    int argument_3);
BattleModel *BattleRenderModel_Init(BattleModel *model);
void func_ov002_0206f1f0(BattleModel *model,
                         BattleSceneResource *resource,
                         u32 component_offset);
extern const char data_ov002_020c0318[];
}

enum BattleStatusPartyAilment3Constant {
    BATTLE_STATUS_AILMENT3_MODEL_OBJECT_DATA_ID = 9,
    BATTLE_STATUS_AILMENT3_TRANSITION_ANIMATION = 29,
    BATTLE_STATUS_AILMENT3_ACTIVE_ANIMATION = 30,
    BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE = 44,
    BATTLE_STATUS_AILMENT3_IMPACT_EFFECT = 740,
    BATTLE_STATUS_AILMENT3_MODEL_SCALE = 256
};
enum BattleStatusPartyAilment3Sound {
    BATTLE_STATUS_AILMENT3_START_MARIO = 0x401C,
    BATTLE_STATUS_AILMENT3_START_LUIGI = 0x4034,
    BATTLE_STATUS_AILMENT3_START_BABY_MARIO = 0x4060,
    BATTLE_STATUS_AILMENT3_START_BABY_LUIGI = 0x4073,
    BATTLE_STATUS_AILMENT3_ACTIVE_MARIO = 0x8009,
    BATTLE_STATUS_AILMENT3_ACTIVE_LUIGI = 0x8010,
    BATTLE_STATUS_AILMENT3_ACTIVE_BABY_MARIO = 0x801A,
    BATTLE_STATUS_AILMENT3_ACTIVE_BABY_LUIGI = 0x8020
};
typedef struct BattleStatusPartyAilment3Task
    BattleStatusPartyAilment3Task;

typedef union BattleStatusPartyAilment3Flags {
    u16 raw;
    struct {
        u16 active_animation_started : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusPartyAilment3Flags;

typedef struct BattleStatusPartyAilment3Payload {
    BattleModel *model;
    u16 actor_id;
    BattleStatusPartyAilment3Flags flags;
} BattleStatusPartyAilment3Payload;

struct BattleStatusPartyAilment3Task {
    BattleStatusPartyAilment3Task *next;
    void (*callback)(BattleStatusPartyAilment3Task *task);
    BattleStatusPartyAilment3Task **owner_slot;
    BattleStatusPartyAilment3Payload data;
};

typedef char BattleStatusPartyAilment3Task_SizeCheck[
    sizeof(BattleStatusPartyAilment3Task) == 0x14 ? 1 : -1
];

BattleAITask *BattleStatus_StartPartyAilment3Visual(int actor_id) {
    BattleStatusPartyAilment3Task *task =
        (BattleStatusPartyAilment3Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusPartyAilment3Payload *payload = &task->data;
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(actor_id);
    BattleObjectDataLoadState *load_state;
    BattleModel *active_model;
    BattleModel *model;
    int animation_id;

    payload->flags.bits.active_animation_started = 0;
    payload->actor_id = actor_id;
    model = (BattleModel *)GameHeap_New(
        sizeof(BattleModel),
        BattleContext_GetRuntimeState()->resource_heap_id,
        data_ov002_020c0318, 0);
    if (model != 0) {
        model = BattleRenderModel_Init(model);
    }
    payload->model = model;
    load_state = BattleObjectData_GetLoadState(
        BATTLE_STATUS_AILMENT3_MODEL_OBJECT_DATA_ID);
    func_ov002_0206f1f0(
        payload->model, load_state,
        (load_state->flags.bits.resource_index - 1) << 6);
    payload->model->stop();

    if (!actor->actor.flag_bits.hit_locked) {
        BattleSceneObject_SetAnimation(
            actor->actor.scene_object,
            BATTLE_STATUS_AILMENT3_TRANSITION_ANIMATION, -1);
    }

    active_model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(actor_id));
    animation_id = active_model->animation_id;
    switch (actor->formation_index) {
    case BATTLE_FORMATION_LUIGI:
        animation_id += BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        animation_id += 2 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        animation_id += 3 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
        break;
    case BATTLE_FORMATION_MARIO_CARRYING:
        animation_id += 4 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
        break;
    case BATTLE_FORMATION_LUIGI_CARRYING:
        animation_id += 5 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
        break;
    }
    payload->model->set_primary_animation((u8)animation_id, 0, 1);

    switch (actor->formation_index) {
    case BATTLE_FORMATION_MARIO:
    case BATTLE_FORMATION_MARIO_CARRYING:
        BattleSound_Play(BATTLE_STATUS_AILMENT3_START_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_LUIGI:
    case BATTLE_FORMATION_LUIGI_CARRYING:
        BattleSound_Play(BATTLE_STATUS_AILMENT3_START_LUIGI, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_MARIO:
        BattleSound_Play(BATTLE_STATUS_AILMENT3_START_BABY_MARIO, 0, 0, 0);
        break;
    case BATTLE_FORMATION_BABY_LUIGI:
        BattleSound_Play(BATTLE_STATUS_AILMENT3_START_BABY_LUIGI, 0, 0, 0);
        break;
    }

    task->callback =
        (void (*)(BattleStatusPartyAilment3Task *))
            BattleStatus_UpdatePartyAilment3Visual;
    return (BattleAITask *)task;
}

void BattleStatus_UpdatePartyAilment3Visual(BattleAITask *base_task) {
    BattleStatusPartyAilment3Task *task =
        (BattleStatusPartyAilment3Task *)base_task;
    BattleStatusPartyAilment3Payload *payload = &task->data;
    BattleModel *active_model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(payload->actor_id));
    BattlePartyActor *actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(payload->actor_id);
    BattleStatusState *status = &actor->actor.ailment_group_1;

    if (!payload->flags.bits.active_animation_started &&
        task != (BattleStatusPartyAilment3Task *)status->effect) {
        if (active_model->animation_id !=
            BATTLE_STATUS_AILMENT3_TRANSITION_ANIMATION) {
            task->callback = 0;
        } else if (status->effect != 0) {
            task->callback = 0;
        } else {
            int animation_id = BATTLE_STATUS_AILMENT3_ACTIVE_ANIMATION;
            s16 effect_offset_x;
            s16 effect_offset_y;
            BattlePosition position;

            payload->flags.bits.active_animation_started = 1;
            actor->actor.flags |= BATTLE_ACTOR_FLAG_HIT_LOCK;
            BattleSceneObject_SetAnimation(
                actor->actor.scene_object,
                BATTLE_STATUS_AILMENT3_ACTIVE_ANIMATION, -1);

            switch (actor->formation_index) {
            case BATTLE_FORMATION_MARIO:
            case BATTLE_FORMATION_MARIO_CARRYING:
                BattleSound_Play(BATTLE_STATUS_AILMENT3_ACTIVE_MARIO,
                                 40, 0, 0);
                break;
            case BATTLE_FORMATION_LUIGI:
            case BATTLE_FORMATION_LUIGI_CARRYING:
                BattleSound_Play(BATTLE_STATUS_AILMENT3_ACTIVE_LUIGI,
                                 40, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_MARIO:
                BattleSound_Play(
                    BATTLE_STATUS_AILMENT3_ACTIVE_BABY_MARIO, 57, 0, 0);
                break;
            case BATTLE_FORMATION_BABY_LUIGI:
                BattleSound_Play(
                    BATTLE_STATUS_AILMENT3_ACTIVE_BABY_LUIGI, 57, 0, 0);
                break;
            }

            switch (actor->formation_index) {
            case BATTLE_FORMATION_LUIGI:
                animation_id +=
                    BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
                break;
            case BATTLE_FORMATION_BABY_MARIO:
                animation_id +=
                    2 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
                break;
            case BATTLE_FORMATION_BABY_LUIGI:
                animation_id +=
                    3 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
                break;
            case BATTLE_FORMATION_MARIO_CARRYING:
                animation_id +=
                    4 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
                break;
            case BATTLE_FORMATION_LUIGI_CARRYING:
                animation_id +=
                    5 * BATTLE_STATUS_AILMENT3_MODEL_ANIMATION_STRIDE;
                break;
            }
            payload->model->set_primary_animation(
                (u8)animation_id, 0, 1);
            payload->model->flags =
                payload->model->flags &
                    ~BATTLE_MODEL_ANIMATION_MODE_MASK |
                BATTLE_MODEL_ANIMATION_MODE_STATUS;

            switch (actor->formation_index) {
            case BATTLE_FORMATION_MARIO:
                if (!BattleContext_GetRuntimeState()
                         ->flags.bits.alternate_formation) {
                    effect_offset_x = -8;
                    effect_offset_y = -15;
                } else {
                    effect_offset_x = -9;
                    effect_offset_y = -14;
                }
                break;
            case BATTLE_FORMATION_LUIGI:
                if (!BattleContext_GetRuntimeState()
                         ->flags.bits.alternate_formation) {
                    effect_offset_x = -8;
                    effect_offset_y = -15;
                } else {
                    effect_offset_x = -9;
                    effect_offset_y = -14;
                }
                break;
            case BATTLE_FORMATION_BABY_MARIO:
                if (!BattleContext_GetRuntimeState()
                         ->flags.bits.alternate_formation) {
                    effect_offset_x = -5;
                    effect_offset_y = -8;
                } else {
                    effect_offset_x = -5;
                    effect_offset_y = -9;
                }
                break;
            case BATTLE_FORMATION_BABY_LUIGI:
                if (!BattleContext_GetRuntimeState()
                         ->flags.bits.alternate_formation) {
                    effect_offset_x = -6;
                    effect_offset_y = -8;
                } else {
                    effect_offset_x = -6;
                    effect_offset_y = -10;
                }
                break;
            case BATTLE_FORMATION_MARIO_CARRYING:
                effect_offset_x = -12;
                effect_offset_y = -20;
                break;
            case BATTLE_FORMATION_LUIGI_CARRYING:
                effect_offset_x = -12;
                effect_offset_y = -20;
                break;
            }

            BattleSceneObject *object = actor->actor.scene_object;
            BattlePosition_StoreViewRelative(
                &position, object->x, (s16)(object->y - object->z),
                (s16)(object->effect_anchor_z +
                      16 * (256 - object->y)),
                object->flags.bits.use_raw_position,
                object->flags.bits.use_alternate_model);
            BattleModelEffect_Spawn(
                BATTLE_STATUS_AILMENT3_IMPACT_EFFECT, 0,
                (s16)(position.x + effect_offset_x),
                (s16)(position.y + effect_offset_y), position.z,
                BATTLE_STATUS_AILMENT3_MODEL_SCALE);
        }

        if (task->callback == 0) {
            actor->actor.flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
            if (payload->model != 0) {
                payload->model->unknown_14();
            }
            return;
        }
    }

    if (active_model->flag_bits.panel_animation_trigger &&
        payload->flags.bits.active_animation_started) {
        BattleSceneObject_SetAnimation(actor->actor.scene_object, 0, -1);
        task->callback = 0;
        actor->actor.flags &= ~BATTLE_ACTOR_FLAG_HIT_LOCK;
        if (payload->model != 0) {
            payload->model->unknown_14();
        }
        return;
    }

    if (!actor->actor.status_visual_flag_bits.suppress_status_model) {
        BattlePosition position;
        BattleSceneObject *object = actor->actor.scene_object;
        BattleSpriteTransform *transform;

        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        transform = payload->model->get_sprite_transform();
        transform->x = position.x << 8;
        transform->y = position.y << 8;
        transform->z = position.z;
        payload->model->prepare_render();
        payload->model->draw(0, 0, 0);
    }
}

enum BattleStatusPartyVisualConstant {
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
