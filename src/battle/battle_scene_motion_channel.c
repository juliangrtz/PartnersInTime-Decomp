#include <game/battle_scene.h>

BattleMotionChannel *BattleSceneObject_GetMotionChannel(
    BattleSceneObject *object, int channel_index) {
    return &object->motion_channels[channel_index];
}
