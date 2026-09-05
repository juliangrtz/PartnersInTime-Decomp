#ifndef PIT_GAME_BATTLE_PARTY_H
#define PIT_GAME_BATTLE_PARTY_H

#include <game/battle_ai.h>

struct BattleSceneObject;

#ifdef __cplusplus
extern "C" {
#endif

void BattleParty_UpdateLuigiReloadFinish(BattleAITask *task);
void BattleParty_UpdateLuigiReloadRequestBaby(BattleAITask *task);
void BattleParty_UpdateLuigiReloadWaitRetreat(BattleAITask *task);
void BattleParty_UpdateLuigiReloadStartRetreat(BattleAITask *task);
void BattleParty_UpdateLuigiReloadWaitAnimation(BattleAITask *task);
void BattleParty_UpdateLuigiReloadWaitResource(BattleAITask *task);
void BattleParty_UpdateMarioReloadFinish(BattleAITask *task);
void BattleParty_UpdateMarioReloadRequestBaby(BattleAITask *task);
void BattleParty_UpdateMarioReloadWaitRetreat(BattleAITask *task);
void BattleParty_UpdateMarioReloadStartRetreat(BattleAITask *task);
void BattleParty_UpdateMarioReloadWaitAnimation(BattleAITask *task);
void BattleParty_UpdateMarioReloadWaitResource(BattleAITask *task);
BattleAITask *BattleParty_StartKnockout(struct BattleSceneObject *object);
void BattleParty_UpdateKnockout(BattleAITask *task);

#ifdef __cplusplus
}
#endif

#endif
