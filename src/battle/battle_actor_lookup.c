#include <game/battle_actor.h>
#include <game/battle_ai.h>

enum BattleActorTableOffset {
    BATTLE_PARTY_ACTOR_TABLE_OFFSET = 0x4FF8,
    BATTLE_ENEMY_ACTOR_TABLE_OFFSET = 0x6508
};

BattleActor *BattleActor_GetEnemySlot(int actor_id) {
    return *(BattleActor **)(gBattleContext + BATTLE_ENEMY_ACTOR_TABLE_OFFSET +
                             (actor_id - BATTLE_ACTOR_ENEMY_FIRST) * 4);
}

BattleActor *BattleActor_GetPartySlot(int actor_id) {
    return *(BattleActor **)(gBattleContext + BATTLE_PARTY_ACTOR_TABLE_OFFSET +
                             (actor_id - BATTLE_ACTOR_PARTY_FIRST) * 4);
}

BattleActor *BattleActor_GetById(int actor_id) {
    u16 party_slot = actor_id - BATTLE_ACTOR_PARTY_FIRST;

    if (party_slot < BATTLE_ACTOR_PARTY_COUNT) {
        return *(BattleActor **)(gBattleContext +
                                 BATTLE_PARTY_ACTOR_TABLE_OFFSET +
                                 party_slot * 4);
    }
    return *(BattleActor **)(gBattleContext + BATTLE_ENEMY_ACTOR_TABLE_OFFSET +
                             (u16)(actor_id - BATTLE_ACTOR_ENEMY_FIRST) * 4);
}
