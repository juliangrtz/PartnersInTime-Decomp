#include <game/scene_motion.h>
#include <hardware.h>

typedef struct SceneMotionStep {
    s16 x, y, z;
} SceneMotionStep;
typedef struct SceneVerticalMotionParameters {
    s32 acceleration, speed;
    s16 origin, target;
} SceneVerticalMotionParameters;
typedef char SceneMotionStep_SizeCheck[sizeof(SceneMotionStep) == 6 ? 1 : -1];
typedef char SceneVerticalMotionParameters_SizeCheck[sizeof(SceneVerticalMotionParameters) == 12 ? 1 : -1];

/* MWCC emits function sections in reverse source order. */

SceneMotionChannel *SceneObject_GetMotionChannel(SceneObject *object, int channel_index)
{
    return &object->motion[channel_index];
}

void SceneObject_StopMotionChannel(SceneObject *object, int channel_index, int apply_deferred_delta)
{
    int index;
    SceneMotionChannel *channel;

    if (SceneObject_GetMotionChannel(object, channel_index)->callback == 0) {
        return;
    }

    channel = &object->motion[0];
    index = 0;
    while (1) {
        if (index == SCENE_MOTION_CHANNEL_COUNT) {
            return;
        }
        if (index == channel_index) {
            if (channel->callback != 0) {
                object->drawn_x += channel->frame_delta_x;
                object->drawn_y += channel->frame_delta_y;
                object->drawn_z += channel->frame_delta_z;
                channel->frame_delta_x = 0;
                channel->frame_delta_y = 0;
                channel->frame_delta_z = 0;
                channel->callback = 0;
            }

            while (1) {
                ++index;
                ++channel;
                if (index == SCENE_MOTION_CHANNEL_COUNT || channel->callback != 0) {
                    return;
                }
                if (channel->has_deferred_delta != 0) {
                    if (apply_deferred_delta != 0) {
                        object->x += channel->deferred_delta_x;
                        object->y += channel->deferred_delta_y;
                        object->base_y += channel->deferred_delta_z;
                    }
                    object->drawn_x += channel->deferred_delta_x;
                    object->drawn_y += channel->deferred_delta_y;
                    object->drawn_z += channel->deferred_delta_z;
                    channel->deferred_delta_x = 0;
                    channel->deferred_delta_y = 0;
                    channel->deferred_delta_z = 0;
                    channel->has_deferred_delta = 0;
                }
            }
        }

        if (channel->callback != 0) {
            SceneMotionChannel *stopped = &object->motion[channel_index];

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

void SceneObject_UnlinkMotion(SceneObject *object)
{
    SceneObject *previous = 0;
    SceneObject *current = gSceneMotionObjectList;

    if (current == 0) {
        return;
    }

    do {
        if (current != object) {
            previous = current;
        } else {
            object->drawn_x = object->x;
            object->drawn_y = object->y;
            object->drawn_z = object->base_y;
            if (previous != 0) {
                previous->motion_next = object->motion_next;
            } else {
                gSceneMotionObjectList = object->motion_next;
            }
            return;
        }
        current = current->motion_next;
    } while (current != 0);
}

s16 *SceneObject_BeginMotionChannel(SceneObject *object, int channel_index, int duration,
                                    SceneMotionCallback callback)
{
    SceneObject *current = gSceneMotionObjectList;
    SceneMotionChannel *channel;

    for (;;) {
        if (current == 0) {
            object->motion_next = gSceneMotionObjectList;
            gSceneMotionObjectList = object;
            break;
        }
        if (current == object) {
            break;
        }
        current = current->motion_next;
    }

    channel = SceneObject_GetMotionChannel(object, channel_index);
    if (channel->callback != 0) {
        SceneObject_StopMotionChannel(object, channel_index, 0);
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

void SceneMotion_UpdateAll(void)
{
    SceneObject *previous = 0;
    SceneObject *object = gSceneMotionObjectList;

    while (object != 0) {
        int channel_index = 0;
        SceneMotionChannel *channel = &object->motion[0];

        object->previous_x = object->x;
        object->previous_y = object->y;
        object->previous_z = object->base_y;

        /* These labels preserve the original MWCC loop layout byte-for-byte. */
    scan_channel:
        if (channel_index == SCENE_MOTION_CHANNEL_COUNT) {
            object->drawn_x = object->x;
            object->drawn_y = object->y;
            object->drawn_z = object->base_y;
            if (previous != 0) {
                previous->motion_next = object->motion_next;
            } else {
                gSceneMotionObjectList = object->motion_next;
            }
            goto object_updated;
        }

        if (channel->callback == 0) {
            goto next_channel;
        }

        object->x = object->drawn_x;
        object->y = object->drawn_y;
        object->base_y = object->drawn_z;

        for (; channel_index < SCENE_MOTION_CHANNEL_COUNT; ++channel_index, ++channel) {
            if (channel->callback != 0) {
                channel->frame_delta_x = object->x;
                channel->frame_delta_y = object->y;
                channel->frame_delta_z = object->base_y;
            }

            if (channel->has_deferred_delta != 0) {
                object->x += channel->deferred_delta_x;
                object->y += channel->deferred_delta_y;
                object->base_y += channel->deferred_delta_z;
            }

            if (channel->callback != 0) {
                if (object->time_paused == 0) {
                    channel->elapsed_q8 += object->time_step_adjustment_q8 + 256;
                }
                if (channel->duration != 0 && channel->duration * 256 < channel->elapsed_q8) {
                    channel->elapsed_q8 = channel->duration * 256;
                }

                channel->callback(object, channel);
                channel->frame_delta_x = object->x - channel->frame_delta_x;
                channel->frame_delta_y = object->y - channel->frame_delta_y;
                channel->frame_delta_z = object->base_y - channel->frame_delta_z;

                if (channel->duration != 0 && channel->elapsed_q8 == channel->duration * 256) {
                    SceneObject_StopMotionChannel(object, channel_index, 1);
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
        SceneObject_UpdateTravelDistance(object);
        object = object->motion_next;
    }
}

void SceneObject_AdjustPosition(SceneObject *point, int dx, int dy, int dz)
{
    SceneObject *node = gSceneMotionObjectList;
    for (;;) {
        if (!node) {
            SceneObject_MoveByImmediate(point, dx, dy, dz);
            return;
        }
        if (node == point) {
            SceneObject_AddPositionDelta(point, dx, dy, dz);
            return;
        }
        node = node->motion_next;
    }
}

void SceneObject_AddPositionDelta(SceneObject *point, int dx, int dy, int dz)
{
    point->x = point->x + dx;
    point->y = point->y + dy;
    point->base_y = point->base_y + dz;
    point->drawn_x = point->drawn_x + dx;
    point->drawn_y = point->drawn_y + dy;
    point->drawn_z = point->drawn_z + dz;
}

void SceneObject_MoveByImmediate(SceneObject *point, int dx, int dy, int dz)
{
    point->previous_x = point->x;
    point->previous_y = point->y;
    point->previous_z = point->base_y;
    point->x = point->x + dx;
    point->y = point->y + dy;
    point->base_y = point->base_y + dz;
    point->drawn_x = point->x;
    point->drawn_y = point->y;
    point->drawn_z = point->base_y;
}

void SceneObject_UpdateTravelDistance(SceneObject *point)
{
    int dx = point->x - point->previous_x;
    int dy = point->y - point->previous_y;
    int dz = point->base_y - point->previous_z;
    *rSQRTCNT = 0;
    *rSQRT_PARAM_L = (dx * dx + dy * dy + dz * dz) << 16;
    while (*rSQRTCNT & 0x8000) {
    }
    point->distance = (point->distance + (s32)*rSQRT_RESULT) / 2;
}

void SceneObject_UpdateMoveBy(SceneObject *point, SceneMotionChannel *motion)
{
    SceneMotionStep *step = (SceneMotionStep *)motion->parameters;
    int t = (motion->elapsed_q8 << 4) / motion->duration;
    point->x = point->x + step->x * t / 4096;
    point->y = point->y + step->y * t / 4096;
    point->base_y = point->base_y + step->z * t / 4096;
}

void SceneObject_MoveBy(SceneObject *point, int owner, int dx, int dy, int dz, int duration)
{
    SceneMotionStep *step;
    if (duration <= 0) {
        point->x = point->x + dx;
        point->y = point->y + dy;
        point->base_y = point->base_y + dz;
        point->drawn_x = point->drawn_x + dx;
        point->drawn_y = point->drawn_y + dy;
        point->drawn_z = point->drawn_z + dz;
        return;
    }
    step =
        (SceneMotionStep *)SceneObject_BeginMotionChannel(point, owner, duration, SceneObject_UpdateMoveBy);
    step->x = dx;
    step->y = dy;
    step->z = dz;
}

void SceneObject_UpdateMoveTo(SceneObject *point, SceneMotionChannel *motion)
{
    SceneMotionStep *step = (SceneMotionStep *)motion->parameters;
    int t = (motion->elapsed_q8 << 4) / motion->duration;
    point->x = point->x + (step->x - point->x) * t / 4096;
    point->y = point->y + (step->y - point->y) * t / 4096;
    point->base_y = point->base_y + (step->z - point->base_y) * t / 4096;
}

void SceneObject_MoveTo(SceneObject *point, int owner, int x, int y, int z, int duration)
{
    SceneMotionStep *step;
    if (duration <= 0) {
        SceneObject_AdjustPosition(point, x - point->x, y - point->y, z - point->base_y);
        return;
    }
    step =
        (SceneMotionStep *)SceneObject_BeginMotionChannel(point, owner, duration, SceneObject_UpdateMoveTo);
    step->x = x;
    step->y = y;
    step->z = z;
}

void SceneObject_UpdateVerticalMotion(SceneObject *point, SceneMotionChannel *motion)
{
    SceneVerticalMotionParameters *curve = (SceneVerticalMotionParameters *)motion->parameters;
    int t = motion->elapsed_q8 / 256;
    if (t == motion->duration) {
        point->base_y = curve->target;
        return;
    }
    point->base_y = curve->origin + (curve->acceleration * t + curve->speed) * t / 4096;
}
