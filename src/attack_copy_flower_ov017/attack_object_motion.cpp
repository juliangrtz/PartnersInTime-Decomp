#include <game/battle_scene.h>
#include <game/overlay017_attack.h>

#include "attack_state.h"

enum Overlay17AttackMotionConstant {
    OVERLAY17_EXIT_X = -64,
    OVERLAY17_EXIT_SPEED_BASE_Q8 = 512,
    OVERLAY17_EXIT_SPEED_RANDOM_RANGE = 256,
    OVERLAY17_LAUNCH_TIMING_STEP = 10,
    OVERLAY17_LAUNCH_DURATION_BASE = 32,
    OVERLAY17_LAUNCH_TERMINAL_VELOCITY_BASE = 40,
};

enum Overlay17AttackObjectFlag {
    OVERLAY17_OBJECT_FLAG_MOTION_CONFIGURED = 1 << 14,
    OVERLAY17_OBJECT_FLAG_MOTION_PHASE_MASK = 0x1F << 16,
    OVERLAY17_SCENE_OBJECT_MOTION_MODE_MASK = 0xF << 20,
    OVERLAY17_SCENE_OBJECT_MOTION_MODE_LAUNCH = 2 << 20,
};

extern "C" int Random_NextModulo(int modulus);

/* Metrowerks emits C++ functions in reverse source order. */
void Overlay17Attack_LaunchObject(Overlay17AttackObject *object,
                                  int destination_x, int destination_y,
                                  int start_x, int start_y, int start_z,
                                  int timing_parameter) {
    BattleSceneObject *scene_object = &object->object;
    unsigned int shifted_flags;
    int scaled_timing;
    int duration;

    BattleSceneObject_AdjustPosition(
        scene_object, start_x - scene_object->x,
        start_y - scene_object->y, start_z - scene_object->z);
    shifted_flags = object->flags << 25;
    timing_parameter -=
        OVERLAY17_LAUNCH_TIMING_STEP * (shifted_flags >> 29);
    scaled_timing = timing_parameter * 4;
    duration = scaled_timing + OVERLAY17_LAUNCH_DURATION_BASE +
               (shifted_flags >> 29);
    BattleSceneObject_StartAcceleratedMotionForDuration(
        scene_object, 3, 0, 0, 1, -start_z, duration,
        scaled_timing + OVERLAY17_LAUNCH_TERMINAL_VELOCITY_BASE,
        1);
    BattleSceneObject_MoveBy(
        scene_object, 2, destination_x - scene_object->x,
        destination_y - scene_object->y, 0, duration);
    Overlay17Attack_ConfigureObjectModel(
        object, ((object->flags << 25) >> 29) + 0x200,
        1, 0x7FFF, 0, 0);

    if (object->flag_bits.enabled) {
        BattleModel_SetAlpha(
            BattleSceneObject_GetActiveModel(scene_object), 0, 0);
    }
    object->flags |= OVERLAY17_OBJECT_FLAG_MOTION_CONFIGURED;
    object->flags &= ~OVERLAY17_OBJECT_FLAG_MOTION_PHASE_MASK;
    scene_object->flags.raw =
        (scene_object->flags.raw &
         ~OVERLAY17_SCENE_OBJECT_MOTION_MODE_MASK) |
        OVERLAY17_SCENE_OBJECT_MOTION_MODE_LAUNCH;
    object->flag_bits.state = 1;
}

void Overlay17Attack_StartHorizontalExit(Overlay17AttackObject *object,
                                         int motion_variant) {
    BattleSceneObject *scene_object = &object->object;
    int speed_q8 =
        Random_NextModulo(OVERLAY17_EXIT_SPEED_RANDOM_RANGE) +
        OVERLAY17_EXIT_SPEED_BASE_Q8;
    int delta_x = OVERLAY17_EXIT_X - scene_object->x;
    int absolute_distance = delta_x > 0 ? delta_x : -delta_x;
    int duration = (absolute_distance << 8) / speed_q8;

    BattleSceneObject_MoveBy(scene_object, 2, delta_x, 0, 0, duration);
    object->motion_variant = motion_variant & 0xF;
    object->flag_bits.state = 3;
}
