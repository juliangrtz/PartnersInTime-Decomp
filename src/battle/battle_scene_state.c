#include <game/battle_scene.h>

void BattleSceneObject_SetStateFlags(BattleSceneObject *object, u8 state,
                                     int independent_flag) {
    object->flags.bits.state = state;
    object->flags.bits.independent_flag = independent_flag;
}
