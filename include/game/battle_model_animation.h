#ifndef PIT_GAME_BATTLE_MODEL_ANIMATION_H
#define PIT_GAME_BATTLE_MODEL_ANIMATION_H

#include <game/battle_scene.h>
#include <game/matrix_animation.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Positive object IDs resolve to alternate models. Nonpositive values are sentinels. */
BattleModel *BattleModelAnimation_ResolveSource(int source, BattleModel **resolved_source);

int BattleModelAnimation_StartInFreeSlot(int animation, BattleModel *model, int x, s16 y, s16 z, int speed);

/* -1 clears the first slot and preserves each of the other three slots. */
BattleModel **BattleModelAnimation_SetModels(BattleModel *first, BattleModel *second,
    BattleModel *third, BattleModel *fourth);
GameMatrixAnimationTrack *BattleModelAnimation_Start(int animation,
    BattleModel *model, int x, int y, s16 z, int speed);
GameMatrixAnimationTrack *BattleModelAnimation_StartAttached(
    GameMatrixAnimationTrack **owner, int animation, BattleModel *model,
    int x, s16 y, s16 z, int speed);

#ifdef __cplusplus
}
#endif

#endif
