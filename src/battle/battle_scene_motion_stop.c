#include <game/battle_scene.h>

int BattleSceneObject_StopMotionChannel(
    BattleSceneObject *object, int channel_index,
    int apply_deferred_delta) {
    BattleMotionChannel *channel;
    int index;

    if ((int)BattleSceneObject_GetMotionChannel(
            object, channel_index)->callback == 0) {
        /* The original ABI leaves the zero callback value in the result. */
        return;
    }

    channel = &object->motion_channels[0];
    index = 0;
    while (1) {
        if (index == BATTLE_MOTION_CHANNEL_COUNT) {
            return (int)channel;
        }
        if (index == channel_index) {
            if (channel->callback != 0) {
                object->motion_target_x += channel->frame_delta_x;
                object->motion_target_y += channel->frame_delta_y;
                object->motion_target_z += channel->frame_delta_z;
                channel->frame_delta_x = 0;
                channel->frame_delta_y = 0;
                channel->frame_delta_z = 0;
                channel->callback = 0;
            }

            while (1) {
                ++index;
                ++channel;
                if (index == BATTLE_MOTION_CHANNEL_COUNT ||
                    channel->callback != 0) {
                    return (int)channel;
                }
                if (channel->has_deferred_delta != 0) {
                    if (apply_deferred_delta != 0) {
                        object->x += channel->deferred_delta_x;
                        object->y += channel->deferred_delta_y;
                        object->z += channel->deferred_delta_z;
                    }
                    object->motion_target_x += channel->deferred_delta_x;
                    object->motion_target_y += channel->deferred_delta_y;
                    object->motion_target_z += channel->deferred_delta_z;
                    channel->deferred_delta_x = 0;
                    channel->deferred_delta_y = 0;
                    channel->deferred_delta_z = 0;
                    channel->has_deferred_delta = 0;
                }
            }
        }

        if (channel->callback != 0) {
            BattleMotionChannel *stopped =
                &object->motion_channels[channel_index];

            stopped->deferred_delta_x = stopped->frame_delta_x;
            stopped->deferred_delta_y = stopped->frame_delta_y;
            stopped->deferred_delta_z = stopped->frame_delta_z;
            stopped->frame_delta_x = 0;
            stopped->frame_delta_y = 0;
            stopped->frame_delta_z = 0;
            stopped->has_deferred_delta = 1;
            stopped->callback = 0;
            return 1;
        }

        ++channel;
        ++index;
    }
}
