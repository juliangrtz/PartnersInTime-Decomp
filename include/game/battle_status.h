#ifndef PIT_GAME_BATTLE_STATUS_H
#define PIT_GAME_BATTLE_STATUS_H

#include <game/battle_actor.h>

enum BattleStatusId {
    BATTLE_STATUS_NONE = 0,
    BATTLE_STATUS_AILMENT_1 = 1,
    BATTLE_STATUS_AILMENT_2 = 2,
    BATTLE_STATUS_AILMENT_3 = 3,
    BATTLE_STATUS_AILMENT_4 = 4,
    BATTLE_STATUS_AILMENT_5 = 5,
    BATTLE_STATUS_POWER_CHANGE = 6,
    BATTLE_STATUS_DEFENSE_CHANGE = 7,
    BATTLE_STATUS_SPEED_CHANGE = 8,
    BATTLE_STATUS_COUNT = 9
};

#ifdef __cplusplus
extern "C" {
#endif

int BattleStatus_ClearEffect(BattleActor *actor, int status_id);
void BattleStatus_ClearAll(BattleActor *actor);
void BattleStatus_StopActorEffect(int actor_id, int status_id);
int BattleStatus_TryApply(BattleActor *actor, int status_id, s16 duration,
                          int magnitude_percent, int chance_percent);
void BattleStatus_SpawnEffect(int actor_id, int status_id);
struct BattleAITask *BattleStatus_StartPartyAilment4Visual(int actor_id);
struct BattleAITask *BattleStatus_StartPartyAilment5Visual(int actor_id);
void BattleStatus_UpdatePartyAilmentVisual(struct BattleAITask *task);
struct BattleAITask *BattleStatus_StartPartyAilment3Visual(int actor_id);
void BattleStatus_UpdatePartyAilment3Visual(struct BattleAITask *task);
struct BattleAITask *BattleStatus_StartPartyAilment2Visual(int actor_id);
u32 BattleStatus_UpdatePartyAilment2Visual(struct BattleAITask *task);
struct BattleAITask *BattleStatus_StartPartyStatVisual(int actor_id,
                                                       int status_id);
void BattleStatus_UpdatePartyStatVisuals(struct BattleAITask *task);

#ifdef __cplusplus
}
#endif

#endif
