#include <game/battle_scene.h>

void BattleSceneObject_UnlinkMotion(BattleSceneObject *object) {
    BattleSceneObject *previous = 0;
    BattleSceneObject *current = gBattleMotionObjectList;

    if (current == 0) {
        return;
    }

    do {
        if (current != object) {
            previous = current;
        } else {
            object->motion_target_x = object->x;
            object->motion_target_y = object->y;
            object->motion_target_z = object->z;
            if (previous != 0) {
                previous->motion_next = object->motion_next;
            } else {
                gBattleMotionObjectList = object->motion_next;
            }
            return;
        }
        current = current->motion_next;
    } while (current != 0);
}

s16 *BattleSceneObject_BeginMotionChannel(BattleSceneObject *object,
                                          int channel_index, int duration,
                                          BattleMotionCallback callback) {
    BattleSceneObject *current = gBattleMotionObjectList;
    BattleMotionChannel *channel;

    for (;;) {
        if (current == 0) {
            object->motion_next = gBattleMotionObjectList;
            gBattleMotionObjectList = object;
            break;
        }
        if (current == object) {
            break;
        }
        current = current->motion_next;
    }

    channel = BattleSceneObject_GetMotionChannel(object, channel_index);
    if (channel->callback != 0) {
        BattleSceneObject_StopMotionChannel(object, channel_index, 0);
    }
    if (channel->has_deferred_delta != 0) {
        channel->deferred_delta_x = 0;
        channel->deferred_delta_y = 0;
        channel->deferred_delta_z = 0;
        channel->has_deferred_delta = 0;
    }
    channel->callback = callback;
    channel->elapsed_q8 = 0;
    channel->duration = duration;
    return channel->parameters;
}
