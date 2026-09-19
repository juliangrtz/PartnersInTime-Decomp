/*
 * Battle scene object motion channels (overlay 2, 0x020A47EC-0x020A50D4).
 *
 * A scene object can be moved by several things at once - a script, an attack
 * and an effect - so movement is split into independent channels. Beginning a
 * channel reserves one, stopping it unlinks it, and the per-frame update pass
 * sums the active channels into the object's position. That is why nothing here
 * writes a position directly: AdjustPosition and AddPositionDelta go through the
 * channel that owns the movement.
 */

#include <game/battle_scene.h>

BattleMotionChannel *BattleSceneObject_GetMotionChannel(
    BattleSceneObject *object, int channel_index) {
    return &object->motion_channels[channel_index];
}

void BattleSceneObject_StopMotionChannel(
    BattleSceneObject *object, int channel_index,
    int apply_deferred_delta) {
    int index;
    BattleMotionChannel *channel;

    if (BattleSceneObject_GetMotionChannel(object, channel_index)->callback == 0) {
        return;
    }

    channel = &object->motion_channels[0];
    index = 0;
    while (1) {
        if (index == BATTLE_MOTION_CHANNEL_COUNT) {
            return;
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
                    return;
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
            return;
        }

        ++channel;
        ++index;
    }
}

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

void BattleSceneObject_UpdateVerticalMotion(
    BattleSceneObject *object, BattleMotionChannel *channel) {
    BattleVerticalMotionParameters *parameters =
        (BattleVerticalMotionParameters *)channel->parameters;
    int frame = channel->elapsed_q8 / 256;

    if (frame == channel->duration) {
        object->z = parameters->target_z;
    } else {
        object->z = parameters->start_z +
                    frame * (parameters->linear_term +
                             parameters->quadratic_term * frame) /
                        4096;
    }
}
