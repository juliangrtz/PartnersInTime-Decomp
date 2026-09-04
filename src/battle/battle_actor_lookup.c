#include <game/battle_actor.h>
#include <game/battle_ai.h>

BattleActor *BattleActor_GetEnemySlot(int actor_id) {
    return *(BattleActor **)(gBattleContext + 0x6508 + (actor_id - 60) * 4);
}

BattleActor *BattleActor_GetPartySlot(int actor_id) {
    return *(BattleActor **)(gBattleContext + 0x4FF8 + (actor_id - 56) * 4);
}

BattleActor *BattleActor_GetById(int actor_id) {
    u16 party_slot = actor_id - 56;

    if (party_slot < 4) {
        return *(BattleActor **)(gBattleContext + 0x4FF8 + party_slot * 4);
    }
    return *(BattleActor **)(gBattleContext + 0x6508 +
                             (u16)(actor_id - 60) * 4);
}
