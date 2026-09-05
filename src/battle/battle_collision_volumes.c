#include <game/battle_effect.h>
#include <game/battle_hit.h>

extern s32 _s32_div_f(s32 numerator, s32 denominator);

/*
 * Interpolate one coordinate using the Q12 collision time.  CodeWarrior's
 * signed division by 0x4000 provides the original round-toward-zero behavior.
 */
#define INTERPOLATE_COLLISION_COORDINATE(start, end, time) \
    ((start) + ((time) * ((end) - (start))) / 0x4000)

int BattleCollision_TestVolumes(BattleCollisionBounds *source_previous,
                                BattleCollisionBounds *source_current,
                                BattleCollisionBounds *target_previous,
                                BattleCollisionBounds *target_current,
                                BattlePosition *hit_position) {
    int source_current_x_min;
    int source_current_x_max;
    int target_current_x_min;
    int target_current_x_max;
    int source_previous_x_min;
    int source_previous_x_max;
    int target_previous_x_min;
    int target_previous_x_max;
    s16 previous_separation[BATTLE_COLLISION_AXIS_COUNT];
    s16 current_separation[BATTLE_COLLISION_AXIS_COUNT];
    int entry_time;
    int collision_axis;
    int axis;
    int exit_time;

    /* Each pair describes the gap from one maximum face to the other box's
     * minimum face.  A negative gap means that face pair is still separated. */
    source_previous_x_min = source_previous->axis[BATTLE_COLLISION_X_MIN];
    target_previous_x_max = target_previous->axis[BATTLE_COLLISION_X_MAX];
    previous_separation[BATTLE_COLLISION_X_MIN] =
        target_previous_x_max - source_previous_x_min;
    target_previous_x_min = target_previous->axis[BATTLE_COLLISION_X_MIN];
    source_previous_x_max = source_previous->axis[BATTLE_COLLISION_X_MAX];
    previous_separation[BATTLE_COLLISION_X_MAX] =
        source_previous_x_max - target_previous_x_min;
    previous_separation[BATTLE_COLLISION_Y_MIN] =
        target_previous->axis[BATTLE_COLLISION_Y_MAX] -
        source_previous->axis[BATTLE_COLLISION_Y_MIN];
    previous_separation[BATTLE_COLLISION_Y_MAX] =
        source_previous->axis[BATTLE_COLLISION_Y_MAX] -
        target_previous->axis[BATTLE_COLLISION_Y_MIN];
    previous_separation[BATTLE_COLLISION_Z_MIN] =
        target_previous->axis[BATTLE_COLLISION_Z_MAX] -
        source_previous->axis[BATTLE_COLLISION_Z_MIN];
    previous_separation[BATTLE_COLLISION_Z_MAX] =
        source_previous->axis[BATTLE_COLLISION_Z_MAX] -
        target_previous->axis[BATTLE_COLLISION_Z_MIN];

    source_current_x_min = source_current->axis[BATTLE_COLLISION_X_MIN];
    target_current_x_max = target_current->axis[BATTLE_COLLISION_X_MAX];
    current_separation[BATTLE_COLLISION_X_MIN] =
        target_current_x_max - source_current_x_min;
    target_current_x_min = target_current->axis[BATTLE_COLLISION_X_MIN];
    source_current_x_max = source_current->axis[BATTLE_COLLISION_X_MAX];
    current_separation[BATTLE_COLLISION_X_MAX] =
        source_current_x_max - target_current_x_min;
    current_separation[BATTLE_COLLISION_Y_MIN] =
        target_current->axis[BATTLE_COLLISION_Y_MAX] -
        source_current->axis[BATTLE_COLLISION_Y_MIN];
    current_separation[BATTLE_COLLISION_Y_MAX] =
        source_current->axis[BATTLE_COLLISION_Y_MAX] -
        target_current->axis[BATTLE_COLLISION_Y_MIN];
    current_separation[BATTLE_COLLISION_Z_MIN] =
        target_current->axis[BATTLE_COLLISION_Z_MAX] -
        source_current->axis[BATTLE_COLLISION_Z_MIN];
    current_separation[BATTLE_COLLISION_Z_MAX] =
        source_current->axis[BATTLE_COLLISION_Z_MAX] -
        target_current->axis[BATTLE_COLLISION_Z_MIN];

    if (previous_separation[BATTLE_COLLISION_X_MIN] < 0 &&
        current_separation[BATTLE_COLLISION_X_MIN] < 0) {
        return 0;
    }
    if (previous_separation[BATTLE_COLLISION_X_MAX] < 0 &&
        current_separation[BATTLE_COLLISION_X_MAX] < 0) {
        return 0;
    }
    if (previous_separation[BATTLE_COLLISION_Y_MIN] < 0 &&
        current_separation[BATTLE_COLLISION_Y_MIN] < 0) {
        return 0;
    }
    if (previous_separation[BATTLE_COLLISION_Y_MAX] < 0 &&
        current_separation[BATTLE_COLLISION_Y_MAX] < 0) {
        return 0;
    }
    if (previous_separation[BATTLE_COLLISION_Z_MIN] < 0 &&
        current_separation[BATTLE_COLLISION_Z_MIN] < 0) {
        return 0;
    }
    if (previous_separation[BATTLE_COLLISION_Z_MAX] < 0 &&
        current_separation[BATTLE_COLLISION_Z_MAX] < 0) {
        return 0;
    }

    if (previous_separation[BATTLE_COLLISION_X_MIN] ==
            current_separation[BATTLE_COLLISION_X_MIN] &&
        previous_separation[BATTLE_COLLISION_X_MAX] ==
            current_separation[BATTLE_COLLISION_X_MAX] &&
        previous_separation[BATTLE_COLLISION_Y_MIN] ==
            current_separation[BATTLE_COLLISION_Y_MIN] &&
        previous_separation[BATTLE_COLLISION_Y_MAX] ==
            current_separation[BATTLE_COLLISION_Y_MAX] &&
        previous_separation[BATTLE_COLLISION_Z_MIN] ==
            current_separation[BATTLE_COLLISION_Z_MIN] &&
        previous_separation[BATTLE_COLLISION_Z_MAX] ==
            current_separation[BATTLE_COLLISION_Z_MAX]) {
        return 0;
    }

    if (previous_separation[BATTLE_COLLISION_X_MIN] > 0 &&
        previous_separation[BATTLE_COLLISION_X_MAX] > 0 &&
        previous_separation[BATTLE_COLLISION_Y_MIN] > 0 &&
        previous_separation[BATTLE_COLLISION_Y_MAX] > 0 &&
        previous_separation[BATTLE_COLLISION_Z_MIN] > 0 &&
        previous_separation[BATTLE_COLLISION_Z_MAX] > 0) {
        return 0;
    }

    entry_time = (s32)0x80000001;
    collision_axis = -1;
    for (axis = 0; axis < BATTLE_COLLISION_AXIS_COUNT; ++axis) {
        int separation = previous_separation[axis];

        if (separation <= 0) {
            int candidate_time = _s32_div_f(
                separation << 12,
                separation - current_separation[axis]);
            if (entry_time < candidate_time) {
                entry_time = candidate_time;
                collision_axis = axis;
            }
        }
    }
    if (collision_axis < 0) {
        return 0;
    }

    exit_time = 0x7FFFFFFF;
    for (axis = 0; axis < BATTLE_COLLISION_AXIS_COUNT; ++axis) {
        int separation = current_separation[axis];

        if (separation <= 0) {
            int previous = previous_separation[axis];
            int candidate_time = _s32_div_f(
                previous << 12, previous - separation);
            if (candidate_time < exit_time) {
                exit_time = candidate_time;
            }
        }
    }
    if (exit_time < entry_time) {
        return 0;
    }

    if (hit_position != 0) {
        int near_face;
        int far_face;

        if (collision_axis == BATTLE_COLLISION_X_MIN) {
            hit_position->x = INTERPOLATE_COLLISION_COORDINATE(
                source_previous_x_min, source_current_x_min, entry_time);
        } else if (collision_axis == BATTLE_COLLISION_X_MAX) {
            hit_position->x = INTERPOLATE_COLLISION_COORDINATE(
                source_previous_x_max, source_current_x_max, entry_time);
        } else {
            near_face = INTERPOLATE_COLLISION_COORDINATE(
                source_previous_x_min, source_current_x_min, entry_time);
            far_face = INTERPOLATE_COLLISION_COORDINATE(
                source_previous_x_max, source_current_x_max, entry_time);
            if (near_face < INTERPOLATE_COLLISION_COORDINATE(
                                target_previous_x_min, target_current_x_min,
                                entry_time)) {
                near_face = INTERPOLATE_COLLISION_COORDINATE(
                    target_previous_x_min, target_current_x_min, entry_time);
            }
            if (INTERPOLATE_COLLISION_COORDINATE(
                    target_previous_x_max, target_current_x_max, entry_time) <
                far_face) {
                far_face = INTERPOLATE_COLLISION_COORDINATE(
                    target_previous_x_max, target_current_x_max, entry_time);
            }
            hit_position->x = (near_face + far_face) / 2;
        }

        if (collision_axis == BATTLE_COLLISION_Y_MIN) {
            hit_position->y = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Y_MIN],
                source_current->axis[BATTLE_COLLISION_Y_MIN], entry_time);
        } else if (collision_axis == BATTLE_COLLISION_Y_MAX) {
            hit_position->y = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Y_MAX],
                source_current->axis[BATTLE_COLLISION_Y_MAX], entry_time);
        } else {
            near_face = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Y_MIN],
                source_current->axis[BATTLE_COLLISION_Y_MIN], entry_time);
            far_face = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Y_MAX],
                source_current->axis[BATTLE_COLLISION_Y_MAX], entry_time);
            if (near_face < INTERPOLATE_COLLISION_COORDINATE(
                                target_previous->axis[BATTLE_COLLISION_Y_MIN],
                                target_current->axis[BATTLE_COLLISION_Y_MIN],
                                entry_time)) {
                near_face = INTERPOLATE_COLLISION_COORDINATE(
                    target_previous->axis[BATTLE_COLLISION_Y_MIN],
                    target_current->axis[BATTLE_COLLISION_Y_MIN], entry_time);
            }
            if (INTERPOLATE_COLLISION_COORDINATE(
                    target_previous->axis[BATTLE_COLLISION_Y_MAX],
                    target_current->axis[BATTLE_COLLISION_Y_MAX], entry_time) <
                far_face) {
                far_face = INTERPOLATE_COLLISION_COORDINATE(
                    target_previous->axis[BATTLE_COLLISION_Y_MAX],
                    target_current->axis[BATTLE_COLLISION_Y_MAX], entry_time);
            }
            hit_position->y = (near_face + far_face) / 2;
        }

        if (collision_axis == BATTLE_COLLISION_Z_MIN) {
            hit_position->z = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Z_MIN],
                source_current->axis[BATTLE_COLLISION_Z_MIN], entry_time);
        } else if (collision_axis == BATTLE_COLLISION_Z_MAX) {
            hit_position->z = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Z_MAX],
                source_current->axis[BATTLE_COLLISION_Z_MAX], entry_time);
        } else {
            near_face = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Z_MIN],
                source_current->axis[BATTLE_COLLISION_Z_MIN], entry_time);
            far_face = INTERPOLATE_COLLISION_COORDINATE(
                source_previous->axis[BATTLE_COLLISION_Z_MAX],
                source_current->axis[BATTLE_COLLISION_Z_MAX], entry_time);
            if (near_face < INTERPOLATE_COLLISION_COORDINATE(
                                target_previous->axis[BATTLE_COLLISION_Z_MIN],
                                target_current->axis[BATTLE_COLLISION_Z_MIN],
                                entry_time)) {
                near_face = INTERPOLATE_COLLISION_COORDINATE(
                    target_previous->axis[BATTLE_COLLISION_Z_MIN],
                    target_current->axis[BATTLE_COLLISION_Z_MIN], entry_time);
            }
            if (INTERPOLATE_COLLISION_COORDINATE(
                    target_previous->axis[BATTLE_COLLISION_Z_MAX],
                    target_current->axis[BATTLE_COLLISION_Z_MAX], entry_time) <
                far_face) {
                far_face = INTERPOLATE_COLLISION_COORDINATE(
                    target_previous->axis[BATTLE_COLLISION_Z_MAX],
                    target_current->axis[BATTLE_COLLISION_Z_MAX], entry_time);
            }
            hit_position->z = (near_face + far_face) / 2;
        }

        /* Collision space stores Y/Z with the opposite sign and order from
         * the position consumed by battle effects. */
        {
            s16 collision_y = hit_position->y;
            hit_position->y = -hit_position->z;
            hit_position->z = -collision_y;
        }
    }

    return 1 << collision_axis;
}

#undef INTERPOLATE_COLLISION_COORDINATE
