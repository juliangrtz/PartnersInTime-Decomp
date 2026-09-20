#ifndef PIT_GAME_BATTLE_ITEM_EFFECT_H
#define PIT_GAME_BATTLE_ITEM_EFFECT_H

#include <game/battle_actor.h>

#ifdef __cplusplus
extern "C" {
#endif
/* hp_after is the result of the item before the active user's badge bonus. */
int BattleItemEffect_ApplyBadgeBoost(BattleActor *target, int hp_after);
#ifdef __cplusplus
}
#endif

#endif
