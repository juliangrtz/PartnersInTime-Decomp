#include <game/battle_scene.h>

void BattleSceneObject_AdjustPosition(BattleSceneObject *object,
                                      int delta_x, int delta_y, int delta_z) {
    BattleSceneObject *moving_object = gBattleMotionObjectList;

    for (;;) {
        if (moving_object == 0) {
            BattleSceneObject_MoveByImmediate(
                object, delta_x, delta_y, delta_z);
            return;
        }
        if (moving_object == object) {
            BattleSceneObject_AddPositionDelta(
                object, delta_x, delta_y, delta_z);
            return;
        }
        moving_object = moving_object->motion_next;
    }
}

void BattleSceneObject_AddPositionDelta(BattleSceneObject *object,
                                        int delta_x, int delta_y, int delta_z) {
    object->x += delta_x;
    object->y += delta_y;
    object->z += delta_z;
    object->motion_target_x += delta_x;
    object->motion_target_y += delta_y;
    object->motion_target_z += delta_z;
}

void BattleSceneObject_MoveByImmediate(BattleSceneObject *object,
                                       int delta_x, int delta_y, int delta_z) {
    object->motion_origin_x = object->x;
    object->motion_origin_y = object->y;
    object->motion_origin_z = object->z;
    object->x += delta_x;
    object->y += delta_y;
    object->z += delta_z;
    object->motion_target_x = object->x;
    object->motion_target_y = object->y;
    object->motion_target_z = object->z;
}

void BattleSceneObject_SnapshotPosition(BattleSceneObject *object) {
    object->motion_origin_x = object->x;
    object->motion_origin_y = object->y;
    object->motion_origin_z = object->z;
}
