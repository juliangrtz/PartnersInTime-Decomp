#ifndef PIT_GAME_BATTLE_MODEL_ANIMATION_H
#define PIT_GAME_BATTLE_MODEL_ANIMATION_H

#include <game/battle_scene.h>
#include <game/matrix_animation.h>

#ifdef __cplusplus
extern "C" {
#endif

GameMatrixAnimationTrack *BattleModelAnimation_Start(int animation,
    BattleModel *model, int x, int y, s16 z, int speed);
GameMatrixAnimationTrack *BattleModelAnimation_StartAttached(
    GameMatrixAnimationTrack **owner, int animation, BattleModel *model,
    int x, s16 y, s16 z, int speed);

#ifdef __cplusplus
}
#endif

#endif
