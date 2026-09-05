#include <game/battle_scene.h>
#include <hardware.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

typedef struct BattleAcceleratedMotionParameters {
    s32 acceleration;
    s32 doubled_initial_velocity;
    s16 distance;
    s16 direction_x_q14;
    s16 direction_y_q14;
    s16 direction_z_q14;
} BattleAcceleratedMotionParameters;

typedef char BattleAcceleratedMotionParameters_SizeCheck[
    sizeof(BattleAcceleratedMotionParameters) == 0x10 ? 1 : -1];

typedef int (*BattlePositionAdjustResult)(
    BattleSceneObject *object, int delta_x, int delta_y, int delta_z);

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
int BattleSceneObject_StartAcceleratedMotion(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int initial_velocity, int acceleration,
    int prefer_longer_duration) {
    int squared_length = direction_x * direction_x +
                         direction_y * direction_y +
                         direction_z * direction_z;
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
            object->z += normalized_z;
            object->motion_target_x += normalized_x;
            object->motion_target_y += normalized_y;
            object->motion_target_z += normalized_z;
            return 0;
        }
        duration = _s32_div_f(distance << 16, initial_velocity);
    } else {
        int discriminant =
            (initial_velocity / 256) * (initial_velocity / 16) +
            ((acceleration * distance) << 5);
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
        second_duration =
            _s32_div_f(-(initial_velocity + root), acceleration);
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
            BattleSceneObject_BeginMotionChannel(
                object, channel_index, 0, 0);
            object->x += normalized_x;
            object->y += normalized_y;
            object->z += normalized_z;
            object->motion_target_x += normalized_x;
            object->motion_target_y += normalized_y;
            object->motion_target_z += normalized_z;
            return 0;
        }
    }

    {
        BattleAcceleratedMotionParameters *parameters =
            (BattleAcceleratedMotionParameters *)
                BattleSceneObject_BeginMotionChannel(
                    object, channel_index, duration,
                    BattleSceneObject_UpdateAcceleratedMotion);
        parameters->acceleration = acceleration;
        parameters->doubled_initial_velocity = 2 * initial_velocity;
        parameters->distance = distance;
        parameters->direction_x_q14 =
            _s32_div_f(direction_x << 14, length);
        parameters->direction_y_q14 =
            _s32_div_f(direction_y << 14, length);
        parameters->direction_z_q14 =
            _s32_div_f(direction_z << 14, length);
    }
    return duration;
}

int BattleSceneObject_StartScaledAcceleratedMotion(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int initial_velocity, int acceleration,
    int prefer_longer_duration) {
    return BattleSceneObject_StartAcceleratedMotion(
        object, channel_index, direction_x, direction_y, direction_z,
        distance, initial_velocity << 8, acceleration << 8,
        prefer_longer_duration);
}

int BattleSceneObject_StartMotionWithPeakDistance(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int peak_distance, int velocity,
    int prefer_longer_duration) {
    int derived_acceleration;

    if (peak_distance == 0) {
        derived_acceleration = 0;
        velocity = -distance;
    } else {
        derived_acceleration =
            _s32_div_f((-velocity) * velocity, 2 * peak_distance);
    }
    return BattleSceneObject_StartAcceleratedMotion(
        object, channel_index, direction_x, direction_y, direction_z,
        distance, velocity << 8, derived_acceleration,
        prefer_longer_duration);
}

int BattleMotion_StartBallistic(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int acceleration, int signed_extent,
    int prefer_longer_duration) {
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
    return BattleSceneObject_StartAcceleratedMotion(
        object, channel_index, direction_x, direction_y, direction_z,
        distance, initial_velocity, acceleration << 8,
        prefer_longer_duration);
}

int BattleSceneObject_StartMotionWithVelocityForDuration(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int duration, int velocity) {
    int squared_length = direction_x * direction_x +
                         direction_y * direction_y +
                         direction_z * direction_z;
    int length;
    BattleAcceleratedMotionParameters *parameters;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;
    parameters = (BattleAcceleratedMotionParameters *)
        BattleSceneObject_BeginMotionChannel(
            object, channel_index, duration,
            BattleSceneObject_UpdateAcceleratedMotion);
    parameters->acceleration = _s32_div_f(
        (distance << 17) - ((duration * velocity) << 9),
        duration * duration);
    parameters->doubled_initial_velocity = velocity << 9;
    parameters->distance = distance;
    parameters->direction_x_q14 =
        _s32_div_f(direction_x << 14, length);
    parameters->direction_y_q14 =
        _s32_div_f(direction_y << 14, length);
    {
        int result = _s32_div_f(direction_z << 14, length);

        parameters->direction_z_q14 = result;
        return result;
    }
}

int BattleSceneObject_StartMotionWithAccelerationForDuration(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int duration, int acceleration) {
    int squared_length = direction_x * direction_x +
                         direction_y * direction_y +
                         direction_z * direction_z;
    int length;
    BattleAcceleratedMotionParameters *parameters;

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = squared_length;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    length = *rSQRT_RESULT;
    parameters = (BattleAcceleratedMotionParameters *)
        BattleSceneObject_BeginMotionChannel(
            object, channel_index, duration,
            BattleSceneObject_UpdateAcceleratedMotion);
    parameters->acceleration = acceleration << 9;
    {
        int velocity = _s32_div_f(distance << 12, duration);

        velocity -= (duration * acceleration) << 4;
        parameters->doubled_initial_velocity = velocity << 5;
    }
    parameters->distance = distance;
    parameters->direction_x_q14 =
        _s32_div_f(direction_x << 14, length);
    parameters->direction_y_q14 =
        _s32_div_f(direction_y << 14, length);
    {
        int result = _s32_div_f(direction_z << 14, length);

        parameters->direction_z_q14 = result;
        return result;
    }
}

int BattleSceneObject_StartAcceleratedMotionForDuration(
    BattleSceneObject *object, int channel_index,
    int direction_x, int direction_y, int direction_z,
    int distance, int duration, int terminal_velocity,
    int positive_acceleration) {
    int squared_length = direction_x * direction_x +
                         direction_y * direction_y +
                         direction_z * direction_z;
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
        return ((BattlePositionAdjustResult)
                    BattleSceneObject_AdjustPosition)(
            object, delta_x, delta_y,
            _s32_div_f(direction_z * distance, length));
    }

    *rSQRTCNT = SQRTCNT_MODE_32;
    *rSQRT_PARAM_L = speed_product << 10;
    while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
    }
    {
        int root = *rSQRT_RESULT;
        BattleAcceleratedMotionParameters *parameters;

        if (!positive_acceleration) {
            root = -root;
        }
        root += terminal_velocity << 5;
        parameters = (BattleAcceleratedMotionParameters *)
            BattleSceneObject_BeginMotionChannel(
                object, channel_index, duration,
                BattleSceneObject_UpdateAcceleratedMotion);
        parameters->acceleration = _s32_div_f(
            ((distance << 4) - root) << 13,
            duration * duration);
        parameters->doubled_initial_velocity =
            _s32_div_f(root << 13, duration);
        parameters->distance = distance;
        parameters->direction_x_q14 =
            _s32_div_f(direction_x << 14, length);
        parameters->direction_y_q14 =
            _s32_div_f(direction_y << 14, length);
        {
            int result = _s32_div_f(direction_z << 14, length);

            parameters->direction_z_q14 = result;
            return result;
        }
    }
}
