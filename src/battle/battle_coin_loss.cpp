#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/save_data.h>

enum BattleCoinLossConstant {
    BATTLE_COIN_TASK_POOL_OFFSET = 0x945C,
    BATTLE_COIN_CAMERA_X_OFFSET = 0xCB9C,
    BATTLE_COIN_CAMERA_Y_OFFSET = 0xCB9E,
    SAVE_COIN_COUNT_OFFSET = 0x488,
    BATTLE_COIN_PARTICLE_OBJECT_FIRST = 41,
    BATTLE_COIN_PARTICLE_OBJECT_COUNT = 3,
    BATTLE_COIN_PARTICLE_SHADOW_RESOURCE = 1,
    BATTLE_COIN_PARTICLE_INITIAL_HEIGHT_Q4 = 256,
    BATTLE_COIN_PARTICLE_INITIAL_INTENSITY = 120,
    BATTLE_COIN_PARTICLE_FAST_FORWARD_LIMIT = 31,
    BATTLE_COIN_PARTICLE_FAST_FORWARD_STEP = 6,
    BATTLE_COIN_PARTICLE_GRAVITY_Q4 = 4,
    BATTLE_COIN_LOSS_SOUND_ID = 243
};

typedef struct BattleCoinParticleState {
    s16 x_q4;
    s16 y_q4;
    s16 height_q4;
    s16 velocity_x_q4;
    s16 velocity_y_q4;
    s16 velocity_height_q4;
    s16 intensity;
    u16 scene_object_id;
} BattleCoinParticleState;

typedef struct BattleCoinParticleTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleCoinParticleState data;
} BattleCoinParticleTask;

typedef struct BattleCoinEmitterState {
    s16 remaining_coins;
    u16 source_object_id;
} BattleCoinEmitterState;

typedef struct BattleCoinEmitterTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleCoinEmitterState data;
} BattleCoinEmitterTask;

typedef char BattleCoinParticleTask_SizeCheck[
    sizeof(BattleCoinParticleTask) == 0x1C ? 1 : -1];
typedef char BattleCoinEmitterTask_SizeCheck[
    sizeof(BattleCoinEmitterTask) == 0x10 ? 1 : -1];

extern "C" {
extern u32 Random_NextModulo(u32 modulus);
extern void func_ov002_020a2fd8(int resource_id, int intensity,
                                int x, int y, int z, int render_flags);

void BattleCoinLoss_UpdateParticle(BattleAITask *base_task);
BattleCoinParticleTask *BattleCoinLoss_SpawnParticle(
    BattleSceneObject *source_object);
void BattleCoinLoss_UpdateEmitter(BattleAITask *base_task);
BattleCoinEmitterTask *BattleCoinLoss_Start(s16 source_object_id,
                                            s16 coin_count);
}

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
BattleCoinEmitterTask *BattleCoinLoss_Start(s16 source_object_id,
                                            s16 coin_count) {
    BattleCoinEmitterTask *task =
        (BattleCoinEmitterTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext + BATTLE_COIN_TASK_POOL_OFFSET),
            0);
    BattleCoinEmitterState *state = &task->data;

    state->source_object_id = source_object_id;
    state->remaining_coins = coin_count;
    task->callback = BattleCoinLoss_UpdateEmitter;
    return task;
}

void BattleCoinLoss_UpdateEmitter(BattleAITask *base_task) {
    BattleCoinEmitterTask *task = (BattleCoinEmitterTask *)base_task;
    BattleCoinEmitterState *state = &task->data;

    if (state->remaining_coins > 0) {
        --state->remaining_coins;
        if (*(u32 *)(gSaveData + SAVE_COIN_COUNT_OFFSET) != 0) {
            --*(u32 *)(gSaveData + SAVE_COIN_COUNT_OFFSET);
            BattleCoinLoss_SpawnParticle(
                BattleSceneObject_GetById(state->source_object_id));
        }
    }
    if (state->remaining_coins <= 0) {
        task->callback = 0;
    }
}

BattleCoinParticleTask *BattleCoinLoss_SpawnParticle(
    BattleSceneObject *source_object) {
    BattleCoinParticleTask *task =
        (BattleCoinParticleTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext + BATTLE_COIN_TASK_POOL_OFFSET),
            0);
    BattleCoinParticleState *state = &task->data;

    state->x_q4 =
        (source_object->x -
         *(s16 *)(gBattleContext + BATTLE_COIN_CAMERA_X_OFFSET) + 8) << 4;
    state->y_q4 =
        (source_object->y -
         *(s16 *)(gBattleContext + BATTLE_COIN_CAMERA_Y_OFFSET)) << 4;
    state->height_q4 = BATTLE_COIN_PARTICLE_INITIAL_HEIGHT_Q4;
    state->velocity_x_q4 = Random_NextModulo(24) + 4;
    state->velocity_y_q4 = Random_NextModulo(12) - 6;
    state->velocity_height_q4 = Random_NextModulo(48) + 56;
    state->intensity = BATTLE_COIN_PARTICLE_INITIAL_INTENSITY;
    state->scene_object_id =
        Random_NextModulo(BATTLE_COIN_PARTICLE_OBJECT_COUNT) +
        BATTLE_COIN_PARTICLE_OBJECT_FIRST;
    task->callback = BattleCoinLoss_UpdateParticle;
    BattleSound_Play(BATTLE_COIN_LOSS_SOUND_ID, 0, 0, 0);
    return task;
}

void BattleCoinLoss_UpdateParticle(BattleAITask *base_task) {
    BattleCoinParticleTask *task = (BattleCoinParticleTask *)base_task;
    BattleCoinParticleState *state = &task->data;
    BattleModel *model = BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(state->scene_object_id));
    BattleSpriteTransform *transform;
    int intensity;

    if (model == 0) {
        task->callback = 0;
        return;
    }
    intensity = state->intensity;
    if (intensity > 0) {
        if (((BattleRuntimeFlags *)(
                 gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET))
                ->bits.fast_forward_effects) {
            if (intensity > BATTLE_COIN_PARTICLE_FAST_FORWARD_LIMIT) {
                state->intensity = BATTLE_COIN_PARTICLE_FAST_FORWARD_LIMIT;
            }
            if (state->intensity < BATTLE_COIN_PARTICLE_FAST_FORWARD_STEP) {
                state->intensity = 0;
            } else {
                state->intensity -= BATTLE_COIN_PARTICLE_FAST_FORWARD_STEP;
            }
        } else {
            state->intensity = intensity - 1;
        }
    } else {
        task->callback = 0;
        return;
    }

    if (intensity > BATTLE_COIN_PARTICLE_FAST_FORWARD_LIMIT) {
        intensity = BATTLE_COIN_PARTICLE_FAST_FORWARD_LIMIT;
    }
    model->animation_state_bits.state = intensity;
    ((BattleRuntimeFlags *)(gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET))
        ->bits.coin_particle_render_pending = 1;

    state->x_q4 += state->velocity_x_q4;
    state->y_q4 += state->velocity_y_q4;
    state->height_q4 += state->velocity_height_q4;
    state->velocity_height_q4 -= BATTLE_COIN_PARTICLE_GRAVITY_Q4;
    if (state->height_q4 <= 0) {
        state->height_q4 = -state->height_q4;
        state->velocity_x_q4 /= 2;
        state->velocity_y_q4 /= 2;
        state->velocity_height_q4 = -state->velocity_height_q4 / 2;
    }

    func_ov002_020a2fd8(
        BATTLE_COIN_PARTICLE_SHADOW_RESOURCE, 7 * intensity / 8,
        state->x_q4 / 16, state->y_q4 / 16, state->height_q4 / 16, 0);
    transform = model->get_sprite_transform();
    transform->x = (state->x_q4 << 8) / 16;
    transform->y =
        ((state->y_q4 - state->height_q4 + 8) << 8) / 16;
    transform->z = 16 * (256 - state->y_q4 / 16);
    model->draw(0, 0, 0);
}
