#include <game/battle_scene.h>
#include <hardware.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

typedef struct BattleVerticalMotionParameters {
    s32 quadratic_term;
    s32 linear_term;
    s16 start_z;
    s16 target_z;
} BattleVerticalMotionParameters;

typedef char BattleVerticalMotionParameters_SizeCheck[
    sizeof(BattleVerticalMotionParameters) == 0x0C ? 1 : -1];

/* Functions in this translation unit are ordered for MWCC's reverse emission. */
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

int BattleSceneObject_StartVerticalArc(
    BattleSceneObject *object, int channel_index,
    int duration_or_velocity, int curve_parameter, int target_z) {
    int velocity = duration_or_velocity;
    int delta_z = target_z - object->z;
    int duration;
    int quadratic_term;
    int root;
    BattleVerticalMotionParameters *parameters;

    if (curve_parameter == 0) {
        duration = duration_or_velocity;
        if (duration_or_velocity == 0) {
            object->z += delta_z;
            object->motion_target_z += delta_z;
            return 0;
        }
        quadratic_term = _s32_div_f(
            delta_z << 12,
            duration_or_velocity * duration_or_velocity);
        velocity = 0;
    } else {
        if (curve_parameter * duration_or_velocity < 0) {
            object->z += delta_z;
            object->motion_target_z += delta_z;
            return 0;
        }
        quadratic_term = _s32_div_f(
            -duration_or_velocity * duration_or_velocity,
            curve_parameter);
        *rSQRTCNT = SQRTCNT_MODE_32;
        *rSQRT_PARAM_L =
            (curve_parameter * (curve_parameter - delta_z)) << 12;
        quadratic_term /= 64;
        while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
        }
        root = *rSQRT_RESULT << 2;
        if (curve_parameter < 0) {
            root = -root;
        }
        duration = _s32_div_f(
            2 * (root + (curve_parameter << 8)), velocity);
    }

    parameters = (BattleVerticalMotionParameters *)
        BattleSceneObject_BeginMotionChannel(
            object, channel_index, duration,
            BattleSceneObject_UpdateVerticalMotion);
    parameters->quadratic_term = quadratic_term;
    parameters->linear_term = velocity << 4;
    parameters->start_z = object->z;
    parameters->target_z = target_z;
    return duration;
}

int BattleSceneObject_StartVerticalMotionTo(
    BattleSceneObject *object, int channel_index,
    int initial_velocity, int acceleration, int target_z,
    int prefer_longer_duration) {
    int delta_z = target_z - object->z;
    int duration;
    int discriminant;
    int root;
    int first_duration;
    int second_duration;
    BattleVerticalMotionParameters *parameters;

    if (acceleration == 0) {
        if (delta_z * initial_velocity < 0) {
            object->z += delta_z;
            object->motion_target_z += delta_z;
            return 0;
        }
        duration = _s32_div_f(delta_z << 8, initial_velocity);
    } else {
        discriminant = initial_velocity * initial_velocity +
                       ((acceleration * delta_z) << 9);

        if (discriminant < 0) {
            object->z += delta_z;
            object->motion_target_z += delta_z;
            return 0;
        }
        *rSQRTCNT = SQRTCNT_MODE_32;
        *rSQRT_PARAM_L = discriminant;
        while ((*rSQRTCNT & SQRTCNTF_BUSY) != 0) {
        }
        root = *rSQRT_RESULT;
        first_duration = _s32_div_f(root - initial_velocity, acceleration);
        second_duration =
            _s32_div_f(-(initial_velocity + root), acceleration);
        duration = second_duration;
        if (prefer_longer_duration) {
            if (first_duration >= duration) {
                duration = first_duration;
            }
        } else {
            if (first_duration >= duration) {
                first_duration = duration;
            }
            duration = first_duration;
        }
        if (duration <= 0) {
            object->z += delta_z;
            object->motion_target_z += delta_z;
            return 0;
        }
    }

    parameters = (BattleVerticalMotionParameters *)
        BattleSceneObject_BeginMotionChannel(
            object, channel_index, duration,
            BattleSceneObject_UpdateVerticalMotion);
    parameters->quadratic_term = (acceleration << 4) / 2;
    parameters->linear_term = initial_velocity << 4;
    parameters->start_z = object->z;
    parameters->target_z = target_z;
    return duration;
}
