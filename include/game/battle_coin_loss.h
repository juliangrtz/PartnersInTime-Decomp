#ifndef PIT_GAME_BATTLE_COIN_LOSS_H
#define PIT_GAME_BATTLE_COIN_LOSS_H

/*
 * The coins scattered when the party takes a hit: the emitter task and the
 * per-particle task it spawns.
 */

#include <game/battle_ai.h>
#include <game/battle_scene.h>
#include <nitro.h>

typedef struct BattleCoinEmitterTask BattleCoinEmitterTask;
typedef struct BattleCoinParticleTask BattleCoinParticleTask;

#ifdef __cplusplus
extern "C" {
#endif

BattleCoinEmitterTask *BattleCoinLoss_Start(u16 source_object_id,
                                            int coin_count);
BattleCoinParticleTask *BattleCoinLoss_SpawnParticle(
    BattleSceneObject *source_object);

#ifdef __cplusplus
}
#endif

#endif
