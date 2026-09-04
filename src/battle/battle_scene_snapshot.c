#include <game/battle_scene.h>

void BattleSceneObject_SnapshotPosition(BattleSceneObject *object) {
    object->motion_origin_x = object->x;
    object->motion_origin_y = object->y;
    object->motion_origin_z = object->z;
}
