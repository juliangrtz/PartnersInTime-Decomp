#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/battle_status.h>

enum BattleStatusEnemyAilment45Constant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
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
