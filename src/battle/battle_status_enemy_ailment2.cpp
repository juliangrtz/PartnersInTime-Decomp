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
