#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

enum BattleObjectEffectConstant {
    BATTLE_OBJECT_EFFECT_TASK_POOL_OFFSET = 0xC76C,
    BATTLE_OBJECT_EFFECT_RESOURCE_ID = 19,
    BATTLE_OBJECT_EFFECT_SCALE = 256,
    BATTLE_OBJECT_EFFECT_DELAYED_Z_OFFSET = 12
};

typedef struct BattleObjectEffectDelayState {
    u16 animation_id;
    s16 delay;
    u16 object_id;
    u16 padding_06;
} BattleObjectEffectDelayState;

typedef struct BattleObjectEffectDelayTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleObjectEffectDelayState data;
} BattleObjectEffectDelayTask;

typedef char BattleObjectEffectDelayTask_SizeCheck[
    sizeof(BattleObjectEffectDelayTask) == 0x14 ? 1 : -1];

void BattleObjectEffect_UpdateDelayed(BattleAITask *base_task);

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
void BattleObjectEffect_SpawnDelayed(int animation_id, int delay,
                                     BattleSceneObject *object) {
    if (delay <= 0) {
        BattlePosition position;

        BattlePosition_StoreViewRelative(
            &position,
            object->x,
            (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        BattleModelEffect_SpawnFromResource(
            BATTLE_OBJECT_EFFECT_RESOURCE_ID,
            animation_id,
            position.x,
            position.y,
            position.z,
            BATTLE_OBJECT_EFFECT_SCALE);
    } else {
        BattleObjectEffectDelayTask *task =
            (BattleObjectEffectDelayTask *)BattleTaskList_Insert(
                (BattleTaskPool *)(gBattleContext +
                                   BATTLE_OBJECT_EFFECT_TASK_POOL_OFFSET),
                0);
        BattleObjectEffectDelayState *data = &task->data;

        data->delay = delay;
        data->animation_id = animation_id;
        data->object_id = object->actor_id;
        task->callback = BattleObjectEffect_UpdateDelayed;
    }
}

void BattleObjectEffect_UpdateDelayed(BattleAITask *base_task) {
    BattleObjectEffectDelayTask *task =
        (BattleObjectEffectDelayTask *)base_task;
    BattleObjectEffectDelayState *data = &task->data;

    data->delay--;
    if (data->delay <= 0) {
        BattlePosition position;
        BattleSceneObject *object =
            BattleSceneObject_GetById(data->object_id);

        BattlePosition_StoreViewRelative(
            &position,
            object->x,
            (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        BattleModelEffect_SpawnFromResource(
            BATTLE_OBJECT_EFFECT_RESOURCE_ID,
            data->animation_id,
            position.x,
            position.y,
            (s16)(position.z - BATTLE_OBJECT_EFFECT_DELAYED_Z_OFFSET),
            BATTLE_OBJECT_EFFECT_SCALE);
        task->callback = 0;
    }
}
