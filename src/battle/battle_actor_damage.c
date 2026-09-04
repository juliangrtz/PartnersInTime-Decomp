#include <game/battle_actor.h>
#include <game/battle_scene.h>

int BattleActor_ApplyDamage(BattleSceneObject *object, int damage) {
    BattleActor *actor;

    if (damage < 1) {
        return 0;
    }

    actor = BattleActor_GetById(object->actor_id);
    actor->hit_state = 0;
    if (actor->current_hp <= 0) {
        actor->current_hp = 0;
        actor->flags |= 0x100;
        return 0;
    }

    if (actor->current_hp <= damage) {
        actor->current_hp = 0;
        actor->flags |= 0x100;
        return 1;
    }
    actor->current_hp -= damage;
    return 0;
}
