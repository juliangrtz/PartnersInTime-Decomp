#include <game/battle_actor.h>
#include <game/battle_scene.h>

extern u8 *gSaveData;

int BattleActor_CanReceiveStatus(BattleActor *actor) {
    u16 actor_id;

    if (actor->current_hp <= 0) {
        return 0;
    }

    actor_id = actor->scene_object->actor_id;
    if (actor_id >= 60) {
        return actor->resource_slot != 0;
    }
    if (actor_id < 58) {
        return 1;
    }
    return *(s16 *)(gSaveData + 0x558) == 2;
}
