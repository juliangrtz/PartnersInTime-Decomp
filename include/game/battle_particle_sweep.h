#ifndef PIT_GAME_BATTLE_PARTICLE_SWEEP_H
#define PIT_GAME_BATTLE_PARTICLE_SWEEP_H
#include <game/battle_ai.h>

typedef struct BattleParticleSweepParameters {
    s16 x, y;
    u16 anchor_object_id, color;
    union {
        u32 motion;
        struct {
            s32 velocity_q8 : 14, acceleration_q8 : 10;
            u32 remainder : 8;
        };
        struct {
            u8 unknown_08[3];
            u8 emission_remainder;
        };
    };
    struct {
        s32 position_q8 : 26;
        u32 alpha : 5, has_active_particles : 1;
    };
} BattleParticleSweepParameters;
typedef struct BattleParticleSweepTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *);
    BattleAITask **owner_slot;
    BattleParticleSweepParameters parameters;
} BattleParticleSweepTask;
typedef char BattleParticleSweepParameters_SizeCheck[sizeof(BattleParticleSweepParameters) == 16 ? 1 : -1];
typedef char BattleParticleSweepTask_SizeCheck[sizeof(BattleParticleSweepTask) == 28 ? 1 : -1];
typedef struct BattleSweepParticleParameters {
    BattleParticleSweepParameters *emitter;
    s16 age;
    u16 angle;
    u8 red, green, blue, size;
} BattleSweepParticleParameters;
typedef struct BattleSweepParticleTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *);
    BattleAITask **owner_slot;
    BattleSweepParticleParameters parameters;
} BattleSweepParticleTask;
typedef char BattleSweepParticleParameters_SizeCheck[sizeof(BattleSweepParticleParameters) == 12 ? 1 : -1];
typedef char BattleSweepParticleTask_SizeCheck[sizeof(BattleSweepParticleTask) == 24 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
BattleAITask *BattleParticleSweep_CreateEmitter(int x, int y, int velocity, int acceleration, u16 color,
                                                u8 alpha, u16 anchor_object_id);
BattleAITask *BattleParticleSweep_CreateParticle(BattleParticleSweepParameters *emitter, u16 angle, int size,
                                                 u8 red, u8 green, u8 blue);
#ifdef __cplusplus
}
#endif
#endif
