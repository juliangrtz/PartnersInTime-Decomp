#ifndef PIT_GAME_BATTLE_SPRITE_GRID_CAPTURE_H
#define PIT_GAME_BATTLE_SPRITE_GRID_CAPTURE_H
#include <game/battle_scene.h>

#ifdef __cplusplus
extern "C" {
#endif

void BattleSpriteGridCapture_Initialize(BattleSceneObject *object);
/* Start chooses count of the sixteen cells without replacement. Both delay
   ranges are stored as signed halfwords for the native render callback. */
void BattleSpriteGridCapture_Start(BattleSceneObject *object, int cell_delay_range,
                                  int quarter_delay_range, int count, int distortion_only);
void BattleSpriteGridCapture_Advance(BattleSceneObject *object, int cell_delay_range,
                                    int quarter_delay_range);
void BattleSpriteGridCapture_Finish(BattleSceneObject *object, int cell_delay_range,
                                   int quarter_delay_range);
/* Phase 5 is a resting state, as is phase 0. */
int BattleSpriteGridCapture_IsActive(BattleSceneObject *object);

#ifdef __cplusplus
}
#endif
#endif
