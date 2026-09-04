#include <game/battle_scene.h>
#include <hardware.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

enum {
    BATTLE_MOTION_FACTOR_SHIFT = 12,
    BATTLE_MOTION_FACTOR_ONE = 1 << BATTLE_MOTION_FACTOR_SHIFT
};

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

void BattleSceneObject_UpdateTravelDistance(BattleSceneObject *object) {
    s32 delta_x = object->x - object->motion_origin_x;
    s32 delta_y = object->y - object->motion_origin_y;
    s32 delta_z = object->z - object->motion_origin_z;
    s32 squared_distance =
        delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_distance << 16;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }

    object->smoothed_travel_distance =
        (object->smoothed_travel_distance + (s32)*rSQRT_RESULT) / 2;
}

void BattleSceneObject_UpdateMoveBy(BattleSceneObject *object,
                                    BattleMotionChannel *channel) {
    s16 *parameters = channel->parameters;
    s32 factor = _s32_div_f(channel->elapsed_q8 << 4, channel->duration);

    object->x += parameters[0] * factor / BATTLE_MOTION_FACTOR_ONE;
    object->y += parameters[1] * factor / BATTLE_MOTION_FACTOR_ONE;
    object->z += parameters[2] * factor / BATTLE_MOTION_FACTOR_ONE;
}

void BattleSceneObject_MoveBy(BattleSceneObject *object, int channel_index,
                              int delta_x, int delta_y, int delta_z,
                              int duration) {
    if (duration <= 0) {
        object->x += delta_x;
        object->y += delta_y;
        object->z += delta_z;
        object->motion_target_x += delta_x;
        object->motion_target_y += delta_y;
        object->motion_target_z += delta_z;
        return;
    }

    {
        s16 *parameters = BattleSceneObject_BeginMotionChannel(
            object, channel_index, duration, BattleSceneObject_UpdateMoveBy);
        parameters[0] = delta_x;
        parameters[1] = delta_y;
        parameters[2] = delta_z;
    }
}

void BattleSceneObject_UpdateMoveTo(BattleSceneObject *object,
                                    BattleMotionChannel *channel) {
    s16 *parameters = channel->parameters;
    s32 factor = _s32_div_f(channel->elapsed_q8 << 4, channel->duration);

    object->x += factor * (parameters[0] - object->x) /
                 BATTLE_MOTION_FACTOR_ONE;
    object->y += factor * (parameters[1] - object->y) /
                 BATTLE_MOTION_FACTOR_ONE;
    object->z += factor * (parameters[2] - object->z) /
                 BATTLE_MOTION_FACTOR_ONE;
}

void BattleSceneObject_MoveTo(BattleSceneObject *object, int channel_index,
                              int target_x, int target_y, int target_z,
                              int duration) {
    if (duration <= 0) {
        BattleSceneObject_AdjustPosition(
            object, target_x - object->x, target_y - object->y,
            target_z - object->z);
        return;
    }

    {
        s16 *parameters = BattleSceneObject_BeginMotionChannel(
            object, channel_index, duration, BattleSceneObject_UpdateMoveTo);
        parameters[0] = target_x;
        parameters[1] = target_y;
        parameters[2] = target_z;
    }
}
