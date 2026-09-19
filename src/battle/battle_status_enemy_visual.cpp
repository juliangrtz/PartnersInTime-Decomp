#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_damage_reaction.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

enum BattleStatusEnemyAilment2Constant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_STATUS_ENEMY_AILMENT2_MODEL_EFFECT = 133,
    BATTLE_STATUS_ENEMY_AILMENT2_MODEL_SCALE = 256
};
typedef struct BattleStatusEnemyAilment2Task
    BattleStatusEnemyAilment2Task;

typedef union BattleStatusEnemyAilment2EffectFlags {
    u16 raw;
    struct {
        u16 active : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusEnemyAilment2EffectFlags;

typedef struct BattleStatusEnemyAilment2Effect {
    u8 unknown_00[4];
    void (*update_callback)(
        struct BattleStatusEnemyAilment2Effect *effect);
    u8 unknown_08[0x0E];
    s16 animation_offset_y;
    u8 unknown_18[2];
    BattleStatusEnemyAilment2EffectFlags flags;
} BattleStatusEnemyAilment2Effect;

typedef struct BattleStatusEnemyAilment2Payload {
    BattleStatusEnemyAilment2Effect *effect;
    u16 actor_id;
    u16 padding_06;
} BattleStatusEnemyAilment2Payload;

struct BattleStatusEnemyAilment2Task {
    BattleStatusEnemyAilment2Task *next;
    u32 (*callback)(BattleStatusEnemyAilment2Task *task);
    BattleStatusEnemyAilment2Task **owner_slot;
    BattleStatusEnemyAilment2Payload data;
};

typedef char BattleStatusEnemyAilment2Task_SizeCheck[
    sizeof(BattleStatusEnemyAilment2Task) == 0x14 ? 1 : -1
];
typedef char BattleStatusEnemyAilment2Effect_SizeCheck[
    sizeof(BattleStatusEnemyAilment2Effect) == 0x1C ? 1 : -1
];

BattleAITask *BattleStatus_StartEnemyAilment2Visual(int actor_id) {
    BattleStatusEnemyAilment2Task *task =
        (BattleStatusEnemyAilment2Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusEnemyAilment2Payload *payload = &task->data;
    BattleEnemyActor *actor =
        (BattleEnemyActor *)BattleActor_GetEnemySlot(actor_id);
    BattleSceneObject *object = actor->actor.scene_object;

    payload->actor_id = actor_id;
    payload->effect =
        (BattleStatusEnemyAilment2Effect *)BattleModelEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT2_MODEL_EFFECT,
            object, object->property_0fc,
            (s16)(object->property_0fd - object->property_0f8),
            (s16)-object->property_0fd,
            BATTLE_STATUS_ENEMY_AILMENT2_MODEL_SCALE);
    payload->effect->flags.bits.active = 1;
    BattleSceneObject_SetAnimation(
        object, actor->hit_animation_id, -1);
    task->callback =
        (u32 (*)(BattleStatusEnemyAilment2Task *))
            BattleStatus_UpdateEnemyAilment2Visual;
    return (BattleAITask *)task;
}

u32 BattleStatus_UpdateEnemyAilment2Visual(BattleAITask *base_task) {
    BattleStatusEnemyAilment2Task *task =
        (BattleStatusEnemyAilment2Task *)base_task;
    BattleStatusEnemyAilment2Payload *payload = &task->data;
    BattleActor *actor = BattleActor_GetEnemySlot(payload->actor_id);
    BattleStatusState *status = &actor->ailment_group_1;
    u32 result = (u32)actor;

    if (payload->effect != 0) {
        payload->effect->animation_offset_y =
            actor->scene_object->property_0fd -
            actor->scene_object->property_0f8;
    }
    if (task ==
        (BattleStatusEnemyAilment2Task *)status->effect) {
        return result;
    }

    if (actor->damage_reaction_task != 0 &&
        actor->damage_reaction_task->callback != 0) {
        actor->scene_object->previous_base_animation_id = 0;
    } else {
        BattleSceneObject_SetAnimation(
            BattleSceneObject_GetById(payload->actor_id), 0, -1);
    }

    result = (u32)payload->effect;
    payload->effect->update_callback = 0;
    task->callback = 0;
    return result;
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

enum BattleStatusEnemyAilment3Constant {
    BATTLE_STATUS_ENEMY_AILMENT3_MODEL_OBJECT_DATA_ID = 8,
    BATTLE_STATUS_ENEMY_AILMENT3_MODEL_SCALE = 256,
    BATTLE_STATUS_ENEMY_AILMENT3_INITIAL_IDLE_FRAME = 512,
    BATTLE_STATUS_ENEMY_AILMENT3_ACTIVE_IDLE_FRAME = 256,
    BATTLE_STATUS_ENEMY_AILMENT3_Z_OFFSET = 15
};
typedef struct BattleStatusEnemyAilment3Task
    BattleStatusEnemyAilment3Task;

typedef union BattleStatusEnemyAilment3Flags {
    u16 raw;
    struct {
        u16 active_animation_started : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusEnemyAilment3Flags;

typedef struct BattleStatusEnemyAilment3Payload {
    BattleModel *model;
    u16 actor_id;
    BattleStatusEnemyAilment3Flags flags;
} BattleStatusEnemyAilment3Payload;

struct BattleStatusEnemyAilment3Task {
    BattleStatusEnemyAilment3Task *next;
    u32 (*callback)(BattleStatusEnemyAilment3Task *task);
    BattleStatusEnemyAilment3Task **owner_slot;
    BattleStatusEnemyAilment3Payload data;
};

typedef char BattleStatusEnemyAilment3Task_SizeCheck[
    sizeof(BattleStatusEnemyAilment3Task) == 0x14 ? 1 : -1
];

BattleAITask *BattleStatus_StartEnemyAilment3Visual(int actor_id) {
    BattleStatusEnemyAilment3Task *task =
        (BattleStatusEnemyAilment3Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusEnemyAilment3Payload *payload = &task->data;
    BattleObjectDataLoadState *load_state;
    BattleModel *model;

    BattleActor_GetEnemySlot(actor_id);
    payload->actor_id = actor_id;
    payload->flags.bits.active_animation_started = 0;

    model = (BattleModel *)GameHeap_New(
        sizeof(BattleModel),
        BattleContext_GetRuntimeState()->resource_heap_id,
        data_ov002_020c0318, 0);
    if (model != 0) {
        model = BattleRenderModel_Init(model);
    }
    payload->model = model;
    load_state = BattleObjectData_GetLoadState(
        BATTLE_STATUS_ENEMY_AILMENT3_MODEL_OBJECT_DATA_ID);
    func_ov002_0206f1f0(
        payload->model, load_state,
        (load_state->flags.bits.resource_index - 1) << 6);
    payload->model->stop();
    payload->model->set_primary_animation(0, 0, 1);
    payload->model->flags &= ~BATTLE_MODEL_ANIMATION_MODE_MASK;

    BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(actor_id))->enemy_idle_frame =
            BATTLE_STATUS_ENEMY_AILMENT3_INITIAL_IDLE_FRAME;
    task->callback =
        (u32 (*)(BattleStatusEnemyAilment3Task *))
            BattleStatus_UpdateEnemyAilment3Visual;
    return (BattleAITask *)task;
}

u32 BattleStatus_UpdateEnemyAilment3Visual(BattleAITask *base_task) {
    BattleStatusEnemyAilment3Task *task =
        (BattleStatusEnemyAilment3Task *)base_task;
    BattleStatusEnemyAilment3Payload *payload = &task->data;
    BattleActor *actor = BattleActor_GetById(payload->actor_id);
    BattleStatusState *status = &actor->ailment_group_1;
    BattleSceneObject *object = actor->scene_object;
    u32 result;

    if (!payload->flags.bits.active_animation_started &&
        task != (BattleStatusEnemyAilment3Task *)status->effect) {
        payload->flags.bits.active_animation_started = 1;
        BattleSceneObject_GetActiveModel(
            BattleSceneObject_GetById(payload->actor_id))->enemy_idle_frame =
                BATTLE_STATUS_ENEMY_AILMENT3_ACTIVE_IDLE_FRAME;
        payload->model->set_primary_animation(1, 0, 1);
        payload->model->flags &= ~BATTLE_MODEL_ANIMATION_MODE_MASK;
    }

    BattlePosition position;
    s16 anchor_y = (s16)(object->y + object->property_0ff);
    s16 anchor_z = (s16)(object->effect_anchor_z -
                         BATTLE_STATUS_ENEMY_AILMENT3_Z_OFFSET);
    s16 object_bottom_y =
        (s16)(object->z + object->property_0fa);
    s16 anchor_x = (s16)(object->x + object->property_0fe);
    BattlePosition_StoreViewRelative(
        &position, anchor_x,
        (s16)(anchor_y - object_bottom_y),
        (s16)(anchor_z + 16 * (256 - anchor_y)),
        object->flags.bits.use_raw_position,
        object->flags.bits.use_alternate_model);

    BattleSpriteTransform *transform = payload->model->get_sprite_transform();
    transform->x = position.x << 8;
    transform->y = position.y << 8;
    transform->z = position.z;
    payload->model->prepare_render();
    payload->model->draw(0, 0, 0);

    result = payload->flags.bits.active_animation_started;
    if (!result) {
        return result;
    }
    result = (u32)payload->model;
    if (!payload->model->flag_bits.panel_animation_trigger) {
        return result;
    }
    if (payload->model != 0) {
        payload->model->unknown_14();
    }
    task->callback = 0;
    return 0;
}

enum BattleStatusEnemyAilment45Constant {
    BATTLE_STATUS_ENEMY_AILMENT4_MODEL_EFFECT = 524,
    BATTLE_STATUS_ENEMY_AILMENT5_MODEL_EFFECT = 758,
    BATTLE_STATUS_ENEMY_AILMENT_END_SPRITE_EFFECT = 359,
    BATTLE_STATUS_ENEMY_AILMENT_END_MODEL_EFFECT = 526,
    BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE = 256
};
typedef struct BattleStatusEnemyAilment45Task
    BattleStatusEnemyAilment45Task;

typedef union BattleStatusEnemyAilment45EffectFlags {
    u16 raw;
    struct {
        u16 active : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusEnemyAilment45EffectFlags;

typedef struct BattleStatusEnemyAilment45Effect {
    u8 unknown_00[4];
    void (*update_callback)(
        struct BattleStatusEnemyAilment45Effect *effect);
    u8 unknown_08[0x12];
    BattleStatusEnemyAilment45EffectFlags flags;
} BattleStatusEnemyAilment45Effect;

typedef struct BattleStatusEnemyAilment45Payload {
    BattleStatusEnemyAilment45Effect *effect;
    u16 actor_id;
    u16 padding_06;
} BattleStatusEnemyAilment45Payload;

struct BattleStatusEnemyAilment45Task {
    BattleStatusEnemyAilment45Task *next;
    u32 (*callback)(BattleStatusEnemyAilment45Task *task);
    BattleStatusEnemyAilment45Task **owner_slot;
    BattleStatusEnemyAilment45Payload data;
};

typedef char BattleStatusEnemyAilment45Task_SizeCheck[
    sizeof(BattleStatusEnemyAilment45Task) == 0x14 ? 1 : -1
];
typedef char BattleStatusEnemyAilment45Effect_SizeCheck[
    sizeof(BattleStatusEnemyAilment45Effect) == 0x1C ? 1 : -1
];

BattleAITask *BattleStatus_StartEnemyAilment4Visual(int actor_id) {
    BattleStatusEnemyAilment45Task *task =
        (BattleStatusEnemyAilment45Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusEnemyAilment45Payload *payload = &task->data;
    BattleSceneObject *object = BattleActor_GetById(actor_id)->scene_object;

    payload->actor_id = actor_id;
    payload->effect =
        (BattleStatusEnemyAilment45Effect *)BattleModelEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT4_MODEL_EFFECT,
            object, 0, 0, 0,
            BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE);
    payload->effect->flags.bits.active = 1;
    task->callback =
        (u32 (*)(BattleStatusEnemyAilment45Task *))
            BattleStatus_UpdateEnemyAilment4Visual;
    return (BattleAITask *)task;
}

u32 BattleStatus_UpdateEnemyAilment4Visual(BattleAITask *base_task) {
    BattleStatusEnemyAilment45Task *task =
        (BattleStatusEnemyAilment45Task *)base_task;
    BattleStatusEnemyAilment45Payload *payload = &task->data;
    BattleActor *actor = BattleActor_GetEnemySlot(payload->actor_id);
    BattleSceneObject *object = actor->scene_object;
    u32 result = (u32)actor;

    if (task ==
        (BattleStatusEnemyAilment45Task *)actor->ailment_group_2.effect) {
        return result;
    }

    if (actor->flag_bits.resource_bound) {
        BattlePosition position;

        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z +
                  16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        BattleSpriteEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT_END_SPRITE_EFFECT,
            position.x, position.y, position.z,
            BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE);
        BattleModelEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT_END_MODEL_EFFECT,
            object, 0, 0, 0,
            BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE);
        result = (u32)payload->effect;
        ((BattleStatusEnemyAilment45Effect *)result)->update_callback = 0;
        task->callback = 0;
        return result;
    }

    result = BattleSceneObject_GetActiveModel(
                 BattleSceneObject_GetById(payload->actor_id))
                 ->flag_bits.animation_active;
    if (!result) {
        result = (u32)payload->effect;
        ((BattleStatusEnemyAilment45Effect *)result)->update_callback = 0;
        task->callback = 0;
    }
    return result;
}

BattleAITask *BattleStatus_StartEnemyAilment5Visual(int actor_id) {
    BattleStatusEnemyAilment45Task *task =
        (BattleStatusEnemyAilment45Task *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
    BattleStatusEnemyAilment45Payload *payload = &task->data;
    BattleSceneObject *object = BattleActor_GetById(actor_id)->scene_object;

    payload->actor_id = actor_id;
    payload->effect =
        (BattleStatusEnemyAilment45Effect *)BattleModelEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT5_MODEL_EFFECT,
            object, 0, 0, 0,
            BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE);
    payload->effect->flags.bits.active = 1;
    task->callback =
        (u32 (*)(BattleStatusEnemyAilment45Task *))
            BattleStatus_UpdateEnemyAilment5Visual;
    return (BattleAITask *)task;
}

u32 BattleStatus_UpdateEnemyAilment5Visual(BattleAITask *base_task) {
    BattleStatusEnemyAilment45Task *task =
        (BattleStatusEnemyAilment45Task *)base_task;
    BattleStatusEnemyAilment45Payload *payload = &task->data;
    BattleActor *actor = BattleActor_GetEnemySlot(payload->actor_id);
    BattleSceneObject *object = actor->scene_object;
    u32 result = (u32)actor;

    if (task ==
        (BattleStatusEnemyAilment45Task *)actor->ailment_group_2.effect) {
        return result;
    }

    if (actor->flag_bits.resource_bound) {
        BattlePosition position;

        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z +
                  16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        BattleSpriteEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT_END_SPRITE_EFFECT,
            position.x, position.y, position.z,
            BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE);
        BattleModelEffect_Spawn(
            BATTLE_STATUS_ENEMY_AILMENT_END_MODEL_EFFECT,
            object, 0, 0, 0,
            BATTLE_STATUS_ENEMY_AILMENT_MODEL_SCALE);
        result = (u32)payload->effect;
        ((BattleStatusEnemyAilment45Effect *)result)->update_callback = 0;
        task->callback = 0;
        return result;
    }

    result = BattleSceneObject_GetActiveModel(
                 BattleSceneObject_GetById(payload->actor_id))
                 ->flag_bits.animation_active;
    if (!result) {
        result = (u32)payload->effect;
        ((BattleStatusEnemyAilment45Effect *)result)->update_callback = 0;
        task->callback = 0;
    }
    return result;
}
