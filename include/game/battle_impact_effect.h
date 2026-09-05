#ifndef PIT_GAME_BATTLE_IMPACT_EFFECT_H
#define PIT_GAME_BATTLE_IMPACT_EFFECT_H

#include <game/battle_ai.h>
#include <game/battle_scene.h>

typedef struct BattleImpactParticlePayload {
    s16 scale_x;
    s16 scale_y;
    u8 rotation_z_high;
    u8 animation_id;
    union {
        u16 render_state;
        struct {
            u8 model_property_056;
            u8 render_state_high;
        };
        struct {
            u16 unknown_00_07 : 8;
            u16 size : 5;
            u16 flip_x : 1;
            u16 flip_y : 1;
            u16 unknown_15 : 1;
        } render_bits;
    };
    BattleSceneObject *object;
    s16 x;
    s16 y;
    s16 z;
    s16 frame;
    void *controller;
} BattleImpactParticlePayload;

typedef struct BattleImpactParticleTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleImpactParticlePayload data;
} BattleImpactParticleTask;

typedef struct BattleImpactTrailEmitterPayload {
    union {
        s16 target_scale_q4;
        u16 target_scale_bits;
    };
    u16 lifetime_multiplier;
    union {
        u16 settings;
        struct {
            u16 spawn_interval : 6;
            u16 size_multiplier : 5;
            u16 target_size : 5;
        } setting_bits;
    };
    s8 initial_z_offset;
    s8 z_velocity;
    u16 spawn_delay;
    u16 object_id;
} BattleImpactTrailEmitterPayload;

typedef struct BattleImpactTrailEmitterTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleImpactTrailEmitterPayload data;
} BattleImpactTrailEmitterTask;

typedef struct BattleObjectBurstEmitterPayload {
    u8 unknown_00[4];
    u16 object_id;
} BattleObjectBurstEmitterPayload;

typedef struct BattleObjectBurstEmitterTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleObjectBurstEmitterPayload data;
} BattleObjectBurstEmitterTask;

typedef char BattleImpactParticlePayload_SizeCheck
    [sizeof(BattleImpactParticlePayload) == 0x18 ? 1 : -1];
typedef char BattleImpactTrailEmitterPayload_SizeCheck
    [sizeof(BattleImpactTrailEmitterPayload) == 0x0C ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

BattleAITask *BattleObjectPropertyCurve_Start(
    BattleSceneObject *primary_object, BattleSceneObject *secondary_object,
    s16 duration, int amplitude);
void BattleObjectPropertyCurve_Update(BattleAITask *task);
void BattleObjectBurstEmitter_Update(BattleAITask *task);
void BattleObjectBurstParticle_Update(BattleAITask *task);
void BattleImpactParticle_UpdateModelFrame(BattleAITask *task);
void BattleImpactParticle_UpdateResourceFrame(BattleAITask *task);
void BattleImpactParticle_Render(BattleImpactParticlePayload *particle,
                                 BattleModel *model,
                                 s16 x, s16 y, s16 z);
void BattleImpactParticle_RenderPrimary(BattleImpactParticlePayload *particle);
BattleImpactParticleTask *BattleImpactParticle_CreateFromObject(
    BattleSceneObject *object, void (*callback)(BattleAITask *task));
void BattleImpactTrailEmitter_Update(BattleAITask *task);
BattleAITask *BattleImpactEmitter_Start(
    u16 object_id, int mode, s16 target_scale_q4, u16 lifetime_multiplier,
    int spawn_interval, int size_multiplier, int target_size,
    int initial_z_offset, int z_velocity);

#ifdef __cplusplus
}
#endif

#endif
