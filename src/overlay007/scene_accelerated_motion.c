#include <game/scene_motion.h>
#include <hardware.h>
extern s32 _s32_div_f(s32, s32);
extern void func_ov007_02086560(SceneObject *, SceneMotionChannel *);
typedef struct SceneAcceleratedMotionParameters {
    s32 acceleration;
    s32 doubled_initial_velocity;
    s16 distance;
    s16 direction_x_q14;
    s16 direction_y_q14;
    s16 direction_z_q14;
} SceneAcceleratedMotionParameters;

typedef char
    SceneAcceleratedMotionParameters_SizeCheck[sizeof(SceneAcceleratedMotionParameters) == 0x10 ? 1 : -1];

typedef struct SceneTrackingParameters {
    s16 offset_x, offset_y, offset_z;
    u16 unknown_06;
    SceneObject *target;
} SceneTrackingParameters;
typedef char SceneTrackingParameters_SizeCheck[sizeof(SceneTrackingParameters) == 12 ? 1 : -1];

typedef int (*ScenePositionAdjustResult)(SceneObject *object, int delta_x, int delta_y, int delta_z);

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
int SceneObject_StartAcceleratedMotion(SceneObject *object, int channel_index, int direction_x,
                                       int direction_y, int direction_z, int distance, int initial_velocity,
                                       int acceleration, int prefer_longer_duration)
{
    int squared_length = direction_x * direction_x + direction_y * direction_y + direction_z * direction_z;
    int first_duration;
    int length;
    int duration;
    int normalized_x;
    int normalized_y;
    int normalized_z;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;

    normalized_x = _s32_div_f(direction_x * distance, length);
    normalized_y = _s32_div_f(direction_y * distance, length);
    normalized_z = _s32_div_f(direction_z * distance, length);

    if (acceleration == 0) {
        if (distance * initial_velocity < 0) {
            object->x += normalized_x;
            object->y += normalized_y;
            object->base_y += normalized_z;
            object->drawn_x += normalized_x;
            object->drawn_y += normalized_y;
            object->drawn_z += normalized_z;
            return 0;
        }
        duration = _s32_div_f(distance << 16, initial_velocity);
    } else {
        int discriminant =
            (initial_velocity / 256) * (initial_velocity / 16) + ((acceleration * distance) << 5);
        int root;
        int second_duration;

        if (discriminant <= 0) {
            root = 0;
        } else {
            *rSQRTCNT = SQRTCNT_MODE_32;
            *rSQRT_PARAM_L = discriminant;
            while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
            }
            root = *rSQRT_RESULT << 6;
        }
        first_duration = _s32_div_f(root - initial_velocity, acceleration);
        second_duration = _s32_div_f(-(initial_velocity + root), acceleration);
        if (prefer_longer_duration) {
            if (first_duration >= second_duration) {
                second_duration = first_duration;
            }
        } else {
            if (first_duration >= second_duration) {
                first_duration = second_duration;
            }
            second_duration = first_duration;
        }
        duration = second_duration;
        if (duration <= 0) {
            SceneObject_BeginMotionChannel(object, channel_index, 0, 0);
            object->x += normalized_x;
            object->y += normalized_y;
            object->base_y += normalized_z;
            object->drawn_x += normalized_x;
            object->drawn_y += normalized_y;
            object->drawn_z += normalized_z;
            return 0;
        }
    }

    {
        SceneAcceleratedMotionParameters *parameters =
            (SceneAcceleratedMotionParameters *)SceneObject_BeginMotionChannel(object, channel_index,
                                                                               duration, func_ov007_02086560);
        parameters->acceleration = acceleration;
        parameters->doubled_initial_velocity = 2 * initial_velocity;
        parameters->distance = distance;
        parameters->direction_x_q14 = _s32_div_f(direction_x << 14, length);
        parameters->direction_y_q14 = _s32_div_f(direction_y << 14, length);
        parameters->direction_z_q14 = _s32_div_f(direction_z << 14, length);
    }
    return duration;
}

int SceneObject_StartScaledAcceleratedMotion(SceneObject *object, int channel_index, int direction_x,
                                             int direction_y, int direction_z, int distance,
                                             int initial_velocity, int acceleration,
                                             int prefer_longer_duration)
{
    return SceneObject_StartAcceleratedMotion(object, channel_index, direction_x, direction_y, direction_z,
                                              distance, initial_velocity << 8, acceleration << 8,
                                              prefer_longer_duration);
}

int SceneObject_StartMotionWithPeakDistance(SceneObject *object, int channel_index, int direction_x,
                                            int direction_y, int direction_z, int distance, int peak_distance,
                                            int velocity, int prefer_longer_duration)
{
    int derived_acceleration;

    if (peak_distance == 0) {
        derived_acceleration = 0;
        velocity = -distance;
    } else {
        derived_acceleration = _s32_div_f((-velocity) * velocity, 2 * peak_distance);
    }
    return SceneObject_StartAcceleratedMotion(object, channel_index, direction_x, direction_y, direction_z,
                                              distance, velocity << 8, derived_acceleration,
                                              prefer_longer_duration);
}

int SceneMotion_StartBallistic(SceneObject *object, int channel_index, int direction_x, int direction_y,
                               int direction_z, int distance, int acceleration, int signed_extent,
                               int prefer_longer_duration)
{
    int initial_velocity;

    if (acceleration * signed_extent > 0) {
        acceleration = 0;
        initial_velocity = -distance;
    } else {
        *rSQRTCNT = SQRTCNT_MODE_32;
        *rSQRT_PARAM_L = (-2 * acceleration * signed_extent) << 12;
        while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
        }
        initial_velocity = *rSQRT_RESULT << 6;
        if (signed_extent < 0) {
            initial_velocity = -initial_velocity;
        }
    }
    return SceneObject_StartAcceleratedMotion(object, channel_index, direction_x, direction_y, direction_z,
                                              distance, initial_velocity, acceleration << 8,
                                              prefer_longer_duration);
}

int SceneObject_StartMotionWithVelocityForDuration(SceneObject *object, int channel_index, int direction_x,
                                                   int direction_y, int direction_z, int distance,
                                                   int duration, int velocity)
{
    int squared_length = direction_x * direction_x + direction_y * direction_y + direction_z * direction_z;
    int length;
    SceneAcceleratedMotionParameters *parameters;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;
    parameters = (SceneAcceleratedMotionParameters *)SceneObject_BeginMotionChannel(
        object, channel_index, duration, func_ov007_02086560);
    parameters->acceleration =
        _s32_div_f((distance << 17) - ((duration * velocity) << 9), duration * duration);
    parameters->doubled_initial_velocity = velocity << 9;
    parameters->distance = distance;
    parameters->direction_x_q14 = _s32_div_f(direction_x << 14, length);
    parameters->direction_y_q14 = _s32_div_f(direction_y << 14, length);
    {
        int result = _s32_div_f(direction_z << 14, length);

        parameters->direction_z_q14 = result;
        return result;
    }
}

int SceneObject_StartMotionWithAccelerationForDuration(SceneObject *object, int channel_index,
                                                       int direction_x, int direction_y, int direction_z,
                                                       int distance, int duration, int acceleration)
{
    int squared_length = direction_x * direction_x + direction_y * direction_y + direction_z * direction_z;
    int length;
    SceneAcceleratedMotionParameters *parameters;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;
    parameters = (SceneAcceleratedMotionParameters *)SceneObject_BeginMotionChannel(
        object, channel_index, duration, func_ov007_02086560);
    parameters->acceleration = acceleration << 9;
    {
        int velocity = _s32_div_f(distance << 12, duration);

        velocity -= (duration * acceleration) << 4;
        parameters->doubled_initial_velocity = velocity << 5;
    }
    parameters->distance = distance;
    parameters->direction_x_q14 = _s32_div_f(direction_x << 14, length);
    parameters->direction_y_q14 = _s32_div_f(direction_y << 14, length);
    {
        int result = _s32_div_f(direction_z << 14, length);

        parameters->direction_z_q14 = result;
        return result;
    }
}

int SceneObject_StartAcceleratedMotionForDuration(SceneObject *object, int channel_index, int direction_x,
                                                  int direction_y, int direction_z, int distance,
                                                  int duration, int terminal_velocity,
                                                  int positive_acceleration)
{
    int squared_length = direction_x * direction_x + direction_y * direction_y + direction_z * direction_z;
    int length;
    int speed_product;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;
    speed_product = terminal_velocity * (terminal_velocity - distance);
    if (speed_product < 0) {
        int delta_x = _s32_div_f(direction_x * distance, length);
        int delta_y = _s32_div_f(direction_y * distance, length);

        /* The original ABI forwards the position helper's live R0 value. */
        return ((ScenePositionAdjustResult)SceneObject_AdjustPosition)(
            object, delta_x, delta_y, _s32_div_f(direction_z * distance, length));
    }

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = speed_product << 10;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    {
        int root = *rSQRT_RESULT;
        SceneAcceleratedMotionParameters *parameters;

        if (!positive_acceleration) {
            root = -root;
        }
        root += terminal_velocity << 5;
        parameters = (SceneAcceleratedMotionParameters *)SceneObject_BeginMotionChannel(
            object, channel_index, duration, func_ov007_02086560);
        parameters->acceleration = _s32_div_f(((distance << 4) - root) << 13, duration * duration);
        parameters->doubled_initial_velocity = _s32_div_f(root << 13, duration);
        parameters->distance = distance;
        parameters->direction_x_q14 = _s32_div_f(direction_x << 14, length);
        parameters->direction_y_q14 = _s32_div_f(direction_y << 14, length);
        {
            int result = _s32_div_f(direction_z << 14, length);

            parameters->direction_z_q14 = result;
            return result;
        }
    }
}

void SceneObject_UpdateMoveToObject(SceneObject *object, SceneMotionChannel *channel)
{
    SceneTrackingParameters *parameters = (SceneTrackingParameters *)channel->parameters;
    SceneObject *target = parameters->target;
    int factor = _s32_div_f(channel->elapsed_q8 << 4, channel->duration);
    object->x += factor * (target->x + parameters->offset_x - object->x) / 4096;
    object->y += factor * (target->y + parameters->offset_y - object->y) / 4096;
    object->base_y += factor * (target->base_y + parameters->offset_z - object->base_y) / 4096;
}

void SceneObject_MoveToObject(SceneObject *object, int channel, int offset_x, int offset_y, int offset_z,
                              int duration, SceneObject *target)
{
    int x = offset_x;
    int y = offset_y;
    SceneTrackingParameters *parameters;
    if (duration <= 0) {
        SceneObject_AdjustPosition(object, target->x + offset_x - object->x, target->y + offset_y - object->y,
                                   target->base_y + offset_z - object->base_y);
        return;
    }
    parameters = (SceneTrackingParameters *)SceneObject_BeginMotionChannel(object, channel, duration,
                                                                           SceneObject_UpdateMoveToObject);
    parameters->offset_x = x;
    parameters->offset_y = y;
    parameters->offset_z = offset_z;
    parameters->target = target;
}
