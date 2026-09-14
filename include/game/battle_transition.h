#ifndef PIT_GAME_BATTLE_TRANSITION_H
#define PIT_GAME_BATTLE_TRANSITION_H
#include <game/battle_ai.h>
#ifdef __cplusplus
extern "C" {
#endif
void BattleTransition_ShowBattleLayers(void *unused);
void BattleTransition_PrepareReturn(BattleAITask *task);
BattleAITask *BattleTransition_BeginReturn(BattleAITask *task);
BattleAITask *BattleTransition_BeginEntry(void);
BattleAITask *BattleTransition_BeginExit(void);
void BattleCaptureSurface_UpdateFade(void);
#ifdef __cplusplus
}
#endif
#endif
