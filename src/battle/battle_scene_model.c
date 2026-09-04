#include <game/battle_scene.h>

void *BattleSceneObject_GetActiveModel(BattleSceneObject *object) {
    if (object->flags.bits.use_alternate_model) {
        return object->alternate_model;
    }
    return object->primary_model;
}
