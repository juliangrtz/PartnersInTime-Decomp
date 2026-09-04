#ifndef PIT_GAME_BATTLE_ACTOR_H
#define PIT_GAME_BATTLE_ACTOR_H

#include <nitro.h>

typedef struct BattleActor BattleActor;

struct BattleActor {
    const void *resource;
    s16 max_hp;
    s16 current_hp;
};

int BattleActor_IsHpAtMostQuarter(BattleActor *actor);
BattleActor *BattleActor_GetEnemySlot(int actor_id);
BattleActor *BattleActor_GetPartySlot(int actor_id);
BattleActor *BattleActor_GetById(int actor_id);

#endif
