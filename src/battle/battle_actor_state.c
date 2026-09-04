#include <game/battle_actor.h>
#include <game/battle_scene.h>

extern u8 *gSaveData;

enum {
    SAVE_PARTY_FORM_OFFSET = 0x558,
    PARTY_FORM_ADULTS_AND_BABIES = 2
};

int BattleActor_IsHpAtMostQuarter(BattleActor *actor) {
    return actor->current_hp * 100 <= actor->max_hp * 25;
}

int BattleActor_CanReceiveStatus(BattleActor *actor) {
    u16 actor_id;

    if (actor->current_hp <= 0) {
        return 0;
    }

    actor_id = actor->scene_object->actor_id;
    if (actor_id >= BATTLE_ACTOR_ENEMY_FIRST) {
        return actor->resource_slot != 0;
    }
    if (actor_id < BATTLE_ACTOR_PARTY_FIRST + 2) {
        return 1;
    }
    return *(s16 *)(gSaveData + SAVE_PARTY_FORM_OFFSET) ==
           PARTY_FORM_ADULTS_AND_BABIES;
}
