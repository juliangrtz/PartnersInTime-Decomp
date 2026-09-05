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

enum BattleStatusEnemyStatConstant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
    BATTLE_STATUS_MODEL_OBJECT_DATA_ID = 8,
    BATTLE_STATUS_MODEL_SCALE = 0x100
};

enum BattleStatusEnemyStatSpriteEffect {
    BATTLE_STATUS_DEFENSE_UP_EFFECT = 352,
    BATTLE_STATUS_POWER_UP_EFFECT = 355,
    BATTLE_STATUS_SPEED_UP_EFFECT = 351,
    BATTLE_STATUS_DEFENSE_DOWN_EFFECT = 478,
    BATTLE_STATUS_POWER_DOWN_EFFECT = 477,
    BATTLE_STATUS_SPEED_DOWN_EFFECT = 479,
    BATTLE_STATUS_CYCLE_DEFENSE_DOWN_EFFECT = 357,
    BATTLE_STATUS_CYCLE_DEFENSE_UP_EFFECT = 358,
    BATTLE_STATUS_CYCLE_POWER_DOWN_EFFECT = 360,
    BATTLE_STATUS_CYCLE_POWER_UP_EFFECT = 361,
    BATTLE_STATUS_CYCLE_SPEED_DOWN_EFFECT = 362,
    BATTLE_STATUS_CYCLE_SPEED_UP_EFFECT = 363
};

typedef struct BattleStatusEnemyStatTask BattleStatusEnemyStatTask;

typedef union BattleStatusEnemyStatFlags {
    u16 raw;
    struct {
        u16 initial_effect_active : 1;
        u16 unknown_01_15 : 15;
    } bits;
} BattleStatusEnemyStatFlags;

typedef struct BattleStatusEnemyStatPayload {
    BattleEffect *effect;
    BattleModel *model;
    u16 actor_id;
    s16 next_status_id;
    BattleStatusEnemyStatFlags flags;
} BattleStatusEnemyStatPayload;

struct BattleStatusEnemyStatTask {
    BattleStatusEnemyStatTask *next;
    u32 (*callback)(BattleStatusEnemyStatTask *task);
    BattleStatusEnemyStatTask **owner_slot;
    BattleStatusEnemyStatPayload data;
};

typedef char BattleStatusEnemyStatPayload_SizeCheck[
    sizeof(BattleStatusEnemyStatPayload) == 0x10 ? 1 : -1
];

BattleAITask *BattleStatus_StartEnemyStatVisual(int actor_id,
                                                int status_id) {
    BattleActor *actor = BattleActor_GetById(actor_id);
    BattleSceneObject *object = actor->scene_object;
    BattleStatusEnemyStatTask *task = 0;
    BattleStatusEnemyStatPayload *payload;

    if (actor->power_change.effect != 0) {
        task = (BattleStatusEnemyStatTask *)actor->power_change.effect;
    }
    if (actor->defense_change.effect != 0) {
        task = (BattleStatusEnemyStatTask *)actor->defense_change.effect;
    }
    if (actor->speed_change.effect != 0) {
        task = (BattleStatusEnemyStatTask *)actor->speed_change.effect;
    }

    if (task != 0) {
        payload = &task->data;
    } else {
        task = (BattleStatusEnemyStatTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET),
            0);
        task->data.effect = 0;
        payload = &task->data;
        payload->model = 0;
    }
    payload->actor_id = actor_id;
    payload->next_status_id = status_id;

    int effect_id = 0;
    switch (status_id) {
    case BATTLE_STATUS_POWER_CHANGE:
        if (actor->power_change.kind < 0) {
            effect_id = BATTLE_STATUS_POWER_DOWN_EFFECT;
        }
        if (actor->power_change.kind > 0) {
            effect_id = BATTLE_STATUS_POWER_UP_EFFECT;
        }
        break;
    case BATTLE_STATUS_DEFENSE_CHANGE:
        if (actor->defense_change.kind < 0) {
            effect_id = BATTLE_STATUS_DEFENSE_DOWN_EFFECT;
        }
        if (actor->defense_change.kind > 0) {
            effect_id = BATTLE_STATUS_DEFENSE_UP_EFFECT;
        }
        break;
    case BATTLE_STATUS_SPEED_CHANGE:
        if (actor->speed_change.kind < 0) {
            effect_id = BATTLE_STATUS_SPEED_DOWN_EFFECT;
        }
        if (actor->speed_change.kind > 0) {
            effect_id = BATTLE_STATUS_SPEED_UP_EFFECT;
        }
        break;
    }

    if (effect_id != 0) {
        BattlePosition position;
        int half_height =
            (object->property_0fd +
             ((u32)object->property_0fd >> 31)) << 15;
        s16 anchor_y =
            (s16)(object->y + (half_height >> 16));
        int half_depth =
            (object->property_0f8 +
             ((u32)object->property_0f8 >> 31)) << 15;
        int half_width =
            (object->property_0fc +
             ((u32)object->property_0fc >> 31)) << 15;
        s16 object_bottom_y =
            (s16)(object->z + (half_depth >> 16));
        s16 anchor_x =
            (s16)(object->x + (half_width >> 16));

        BattlePosition_StoreViewRelative(
            &position, anchor_x,
            (s16)(anchor_y - object_bottom_y),
            (s16)(object->effect_anchor_z +
                  16 * (256 - anchor_y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        BattleEffect *effect = BattleSpriteEffect_Spawn(
            effect_id, position.x, position.y, position.z,
            BATTLE_STATUS_MODEL_SCALE);
        if (payload->effect == 0) {
            payload->effect = effect;
            payload->effect->owner_slot = &payload->effect;
        }
        payload->flags.bits.initial_effect_active = 1;
    }

    task->callback =
        (u32 (*)(BattleStatusEnemyStatTask *))
            BattleStatus_UpdateEnemyStatVisuals;
    return (BattleAITask *)task;
}

u32 BattleStatus_UpdateEnemyStatVisuals(BattleAITask *base_task) {
    BattleStatusEnemyStatTask *task =
        (BattleStatusEnemyStatTask *)base_task;
    BattleStatusEnemyStatPayload *payload = &task->data;
    BattleActor *actor = BattleActor_GetById(payload->actor_id);
    BattleSceneObject *object = actor->scene_object;
    u32 result;

    if (payload->model != 0) {
        BattlePosition position;
        BattleSpriteTransform *transform;

        BattlePosition_StoreViewRelative(
            &position, object->x, (s16)(object->y - object->z),
            (s16)(object->effect_anchor_z +
                  16 * (256 - object->y)),
            object->flags.bits.use_raw_position,
            object->flags.bits.use_alternate_model);
        transform = payload->model->get_sprite_transform();
        transform->x = position.x << 8;
        transform->y = position.y << 8;
        transform->z = position.z - 8;
        payload->model->prepare_render();
        payload->model->draw(0, 0, 0);
    }

    if (!BattleContext_GetRuntimeState()->flags.bits.status_visuals_active &&
        payload->effect != 0) {
        result = payload->flags.bits.initial_effect_active;
        if (!result) {
            payload->effect->update_callback = 0;
        }
        return result;
    }
    result = (u32)payload->effect;
    if (payload->effect != 0) {
        return result;
    }

    payload->flags.bits.initial_effect_active = 0;
    BattleStatusEnemyStatTask *active_task = 0;
    if (actor->power_change.effect != 0) {
        active_task =
            (BattleStatusEnemyStatTask *)actor->power_change.effect;
        if (active_task != task) {
            task->callback = 0;
        }
    }
    if (actor->defense_change.effect != 0) {
        active_task =
            (BattleStatusEnemyStatTask *)actor->defense_change.effect;
        if (active_task != task) {
            task->callback = 0;
        }
    }
    if (actor->speed_change.effect != 0) {
        active_task =
            (BattleStatusEnemyStatTask *)actor->speed_change.effect;
        if (active_task != task) {
            task->callback = 0;
        }
    }
    if (active_task == 0) {
        task->callback = 0;
    }

    if (task->callback == 0 ||
        !BattleContext_GetRuntimeState()->flags.bits.status_visuals_active) {
        result = (u32)payload->model;
        if (payload->model != 0) {
            payload->model->unknown_14();
            payload->model = 0;
            return 0;
        }
        return result;
    }

    int status_id = payload->next_status_id;
    do {
        int effect_id = 0;
        int animation_layer = 0;

        switch (status_id) {
        case BATTLE_STATUS_POWER_CHANGE:
            status_id = BATTLE_STATUS_DEFENSE_CHANGE;
            if (actor->power_change.kind < 0) {
                effect_id = BATTLE_STATUS_CYCLE_POWER_DOWN_EFFECT;
            }
            if (actor->power_change.kind > 0) {
                effect_id = BATTLE_STATUS_CYCLE_POWER_UP_EFFECT;
            }
            break;
        case BATTLE_STATUS_DEFENSE_CHANGE:
            status_id = BATTLE_STATUS_SPEED_CHANGE;
            if (actor->defense_change.kind < 0) {
                effect_id = BATTLE_STATUS_CYCLE_DEFENSE_DOWN_EFFECT;
            }
            if (actor->defense_change.kind > 0) {
                effect_id = BATTLE_STATUS_CYCLE_DEFENSE_UP_EFFECT;
            }
            break;
        case BATTLE_STATUS_SPEED_CHANGE:
            status_id = BATTLE_STATUS_POWER_CHANGE;
            if (actor->speed_change.kind < 0) {
                effect_id = BATTLE_STATUS_CYCLE_SPEED_DOWN_EFFECT;
            }
            if (actor->speed_change.kind > 0) {
                effect_id = BATTLE_STATUS_CYCLE_SPEED_UP_EFFECT;
            }
            break;
        }

        switch (effect_id) {
        case BATTLE_STATUS_CYCLE_DEFENSE_DOWN_EFFECT:
            animation_layer = 7;
            break;
        case BATTLE_STATUS_CYCLE_DEFENSE_UP_EFFECT:
            animation_layer = 4;
            break;
        case BATTLE_STATUS_CYCLE_POWER_DOWN_EFFECT:
            animation_layer = 6;
            break;
        case BATTLE_STATUS_CYCLE_POWER_UP_EFFECT:
            animation_layer = 3;
            break;
        case BATTLE_STATUS_CYCLE_SPEED_DOWN_EFFECT:
            animation_layer = 8;
            break;
        case BATTLE_STATUS_CYCLE_SPEED_UP_EFFECT:
            animation_layer = 5;
            break;
        }

        if (payload->model == 0) {
            BattleModel *model = (BattleModel *)func_02029be0(
                sizeof(BattleModel),
                BattleContext_GetRuntimeState()->resource_heap_id,
                data_ov002_020c0318, 0);
            if (model != 0) {
                model = func_02010634(model);
            }
            payload->model = model;
            BattleObjectDataLoadState *load_state =
                BattleObjectData_GetLoadState(
                    BATTLE_STATUS_MODEL_OBJECT_DATA_ID);
            func_ov002_0206f1f0(
                payload->model, load_state,
                (load_state->flags.bits.resource_index - 1) << 6);
            payload->model->stop();
            payload->model->set_primary_animation(
                (u8)animation_layer, 0, 1);
        }

        if (effect_id != 0) {
            BattlePosition position;
            s16 anchor_y =
                (s16)(object->y + object->property_0fd / 2);

            BattlePosition_StoreViewRelative(
                &position,
                (s16)(object->x + object->property_0fc / 2),
                (s16)(anchor_y -
                      (object->z + object->property_0f8 / 2)),
                (s16)(object->effect_anchor_z +
                      16 * (256 - anchor_y)),
                object->flags.bits.use_raw_position,
                object->flags.bits.use_alternate_model);
            payload->effect = BattleSpriteEffect_Spawn(
                effect_id, position.x, position.y, position.z,
                BATTLE_STATUS_MODEL_SCALE);
            payload->effect->owner_slot = &payload->effect;
            payload->next_status_id = status_id;
            return payload->model->set_primary_animation(
                (u8)animation_layer, 0, 1);
        }
    } while (payload->next_status_id != status_id);

    task->callback = 0;
    result = (u32)payload->model;
    if (payload->model != 0) {
        payload->model->unknown_14();
        payload->model = 0;
        return 0;
    }
    return result;
}
