#include <game/battle_actor.h>

int BattleActor_IsHpAtMostQuarter(BattleActor *actor) {
    return actor->current_hp * 100 <= actor->max_hp * 25;
}
