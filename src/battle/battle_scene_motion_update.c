#include <game/battle_scene.h>

void BattleSceneMotion_UpdateAll(void) {
    BattleSceneObject *previous = 0;
    BattleSceneObject *object = gBattleMotionObjectList;

    while (object != 0) {
        int channel_index = 0;
        BattleMotionChannel *channel = &object->motion_channels[0];

        object->motion_origin_x = object->x;
        object->motion_origin_y = object->y;
        object->motion_origin_z = object->z;

        /* These labels preserve the original MWCC loop layout byte-for-byte. */
scan_channel:
        if (channel_index == BATTLE_MOTION_CHANNEL_COUNT) {
            object->motion_target_x = object->x;
            object->motion_target_y = object->y;
            object->motion_target_z = object->z;
            if (previous != 0) {
                previous->motion_next = object->motion_next;
            } else {
                gBattleMotionObjectList = object->motion_next;
            }
            goto object_updated;
        }

        if (channel->callback == 0) {
            goto next_channel;
        }

        object->x = object->motion_target_x;
        object->y = object->motion_target_y;
        object->z = object->motion_target_z;

        for (; channel_index < BATTLE_MOTION_CHANNEL_COUNT;
             ++channel_index, ++channel) {
            if (channel->callback != 0) {
                channel->frame_delta_x = object->x;
                channel->frame_delta_y = object->y;
                channel->frame_delta_z = object->z;
            }

            if (channel->has_deferred_delta != 0) {
                object->x += channel->deferred_delta_x;
                object->y += channel->deferred_delta_y;
                object->z += channel->deferred_delta_z;
            }

            if (channel->callback != 0) {
                if (object->time_paused == 0) {
                    channel->elapsed_q8 +=
                        object->time_step_adjustment_q8 + 256;
                }
                if (channel->duration != 0 &&
                    channel->duration * 256 < channel->elapsed_q8) {
                    channel->elapsed_q8 = channel->duration * 256;
                }

                channel->callback(object, channel);
                channel->frame_delta_x =
                    object->x - channel->frame_delta_x;
                channel->frame_delta_y =
                    object->y - channel->frame_delta_y;
                channel->frame_delta_z =
                    object->z - channel->frame_delta_z;

                if (channel->duration != 0 &&
                    channel->elapsed_q8 == channel->duration * 256) {
                    BattleSceneObject_StopMotionChannel(
                        object, channel_index, 1);
                }
            }
        }
        previous = object;
        goto object_updated;

next_channel:
        ++channel;
        ++channel_index;
        goto scan_channel;

object_updated:
        BattleSceneObject_UpdateTravelDistance(object);
        object = object->motion_next;
    }
}
