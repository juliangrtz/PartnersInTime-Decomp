#ifndef PIT_GAME_BATTLE_EFFECT_CONTROLS_H
#define PIT_GAME_BATTLE_EFFECT_CONTROLS_H
#include <game/battle_scene.h>
#include <game/battle_ai.h>
#ifdef __cplusplus
extern "C" {
#endif
BattleAITask *BattleGroundRipple_Create(BattleSceneObject *object, int radius, int growth_frames,
                                        int hold_frames);
void BattleCaptureOverlay_Start(void);
void BattleChainRig_SetActor(BattleSceneObject *object, int chain, u16 actor_id);
void BattleChainRig_StartSequence(BattleSceneObject *object, int sequence);
#ifdef __cplusplus
}
#endif
#endif
