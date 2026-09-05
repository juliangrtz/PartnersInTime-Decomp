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

enum BattleStatusEnemyAilment3Constant {
    BATTLE_STATUS_VISUAL_TASK_POOL_OFFSET = 0x8B44,
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

    model = (BattleModel *)func_02029be0(
        sizeof(BattleModel),
        BattleContext_GetRuntimeState()->resource_heap_id,
        data_ov002_020c0318, 0);
    if (model != 0) {
        model = func_02010634(model);
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
