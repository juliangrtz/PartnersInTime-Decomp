#ifndef PIT_GAME_BATTLE_RASTER_EFFECTS_H
#define PIT_GAME_BATTLE_RASTER_EFFECTS_H

#include <nitro.h>

typedef struct BattleRasterEffectTask BattleRasterEffectTask;

BattleRasterEffectTask *BattleEffectParticle_Spawn(
    s16 x, s16 y, u16 velocity);
BattleRasterEffectTask *BattleRasterResourceTransition_Start(
    s16 resource_mode, s16 extent, u16 fade_out_duration,
    u16 fade_in_duration);
BattleRasterEffectTask *BattleRasterEffect_StartPersistent(
    s16 value, u16 extent, u16 duration, s16 progress_limit, int mode);
BattleRasterEffectTask *BattleRasterEffect_StartFinite(
    s16 value, u16 extent, u16 duration, s16 progress_limit, int mode);

#endif
