#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

extern "C" {
void *func_02029be0(u32 size, u32 heap_id, const void *allocator,
                    int argument_3);
BattleModel *func_02010634(BattleModel *model);
void func_ov002_0206f1f0(BattleModel *model,
                         BattleSceneResource *resource,
                         u32 component_offset);
extern const char data_ov002_020c0318[];
}

enum BattleStatusPartyAilment3Constant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
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
    model = (BattleModel *)func_02029be0(
        sizeof(BattleModel),
        BattleContext_GetRuntimeState()->resource_heap_id,
        data_ov002_020c0318, 0);
    if (model != 0) {
        model = func_02010634(model);
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
