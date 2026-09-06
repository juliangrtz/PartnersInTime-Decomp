#ifndef PIT_GAME_BATTLE_RASTER_EFFECTS_H
#define PIT_GAME_BATTLE_RASTER_EFFECTS_H

#include <nitro.h>

typedef struct BattleRasterEffectTask BattleRasterEffectTask;

BattleRasterEffectTask *BattleEffectParticle_Spawn(
    int x, int y, int velocity);
BattleRasterEffectTask *BattleRasterResourceTransition_Start(
    int resource_mode, u16 extent, int fade_out_duration,
    int fade_in_duration);
BattleRasterEffectTask *BattleRasterEffect_StartPersistent(
    s16 value, u16 extent, u16 duration, s16 progress_limit, int mode);
BattleRasterEffectTask *BattleRasterEffect_StartFinite(
    int value, u16 extent, int duration, int progress_limit, int mode);

#endif
