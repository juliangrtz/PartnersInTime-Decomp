#include <game/battle_scene.h>

BattleModel *BattleSceneObject_GetActiveModelById(int object_id) {
    return BattleSceneObject_GetActiveModel(
        BattleSceneObject_GetById(object_id));
}

BattleModel *BattleSceneObject_GetActiveModel(BattleSceneObject *object) {
    if (object->flags.bits.use_alternate_model) {
        return object->alternate_model;
    }
    return object->primary_model;
}

void BattleSceneObject_SetModelFlag10ById(int object_id, int enabled) {
    BattleModel *model = BattleSceneObject_GetActiveModelById(object_id);

    model->flags =
        (model->flags & ~BATTLE_MODEL_FLAG_10) |
        ((enabled & 1) << BATTLE_MODEL_FLAG_10_SHIFT);
}

void BattleSceneObject_SetModelFlag10(BattleSceneObject *object, int enabled) {
    BattleSceneObject_SetModelFlag10ById(object->actor_id, enabled);
}

void BattleSceneObject_SetModelFlag11ById(int object_id, int enabled) {
    BattleModel *model = BattleSceneObject_GetActiveModelById(object_id);

    model->flags =
        (model->flags & ~BATTLE_MODEL_FLAG_11) |
        ((enabled & 1) << BATTLE_MODEL_FLAG_11_SHIFT);
}

void BattleSceneObject_SetStateFlags(BattleSceneObject *object, u8 state,
                                     int independent_flag) {
    object->flags.bits.state = state;
    object->flags.bits.independent_flag = independent_flag;
}
