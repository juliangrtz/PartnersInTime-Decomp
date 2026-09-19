/*
 * Copy Flower object (overlay 17, 0x020C33A4-0x020C3748).
 *
 * Launching the object, its horizontal exit, the model configuration it uses,
 * and the idle test the sequence waits on.
 */

#include <game/battle_scene.h>
#include <game/overlay017_attack.h>

#include "attack_state.h"

enum Overlay17AttackMotionConstant {
    OVERLAY17_EXIT_X = -64,
    OVERLAY17_EXIT_SPEED_BASE_Q8 = 512,
    OVERLAY17_EXIT_SPEED_RANDOM_RANGE = 256,
    OVERLAY17_LAUNCH_TIMING_STEP = 10,
    OVERLAY17_LAUNCH_DURATION_BASE = 32,
    OVERLAY17_LAUNCH_TERMINAL_VELOCITY_BASE = 40
};
enum Overlay17AttackObjectFlag {
    OVERLAY17_OBJECT_FLAG_MOTION_CONFIGURED = 1 << 14,
    OVERLAY17_OBJECT_FLAG_MOTION_PHASE_MASK = 0x1F << 16,
    OVERLAY17_SCENE_OBJECT_MOTION_MODE_MASK = 0xF << 20,
    OVERLAY17_SCENE_OBJECT_MOTION_MODE_LAUNCH = 2 << 20
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

#include <game/battle_object.h>

enum Overlay17AttackResourceConstant {
    OVERLAY17_RESOURCE_BASE = 53,
    OVERLAY17_PARTY_RESOURCE_BASE = 57,
    OVERLAY17_BABY_LUIGI_RESOURCE = 59
};
extern "C" {
void BattleSceneObject_SetAnimationMode01(BattleSceneObject *object, int value);
void func_0200940c(BattleModel *model, int scale);
int func_02009224(BattleModel *model, int animation_id);
}

/* Metrowerks emits C++ functions in reverse source order. */
void Overlay17Attack_ConfigureObjectModel(
    Overlay17AttackObject *object, int animation_command,
    int model_value, int render_state,
    int synchronize_animation_layer, int flag_10) {
    BattleSceneObject *scene_object = &object->object;
    BattleModel *model;
    u16 previous_render_state;
    u16 resource_id;
    int resource_mode;

    if (object->flag_bits.enabled == 0) {
        return;
    }

    BattleSceneObject_GetActiveModel(scene_object);
    resource_mode = animation_command & 0xF00;
    switch (resource_mode) {
        case 0:
            resource_id = (u16)(((object->flags << 25) >> 29) +
                                OVERLAY17_RESOURCE_BASE);
            break;
        case 0x100:
            resource_id = (u16)((((object->flags << 25) >> 29) & 1) +
                                OVERLAY17_PARTY_RESOURCE_BASE);
            break;
        case 0x200:
            resource_id = OVERLAY17_BABY_LUIGI_RESOURCE;
            break;
    }

    if (scene_object->resource == 0 ||
        scene_object->resource->object_data_id != resource_id) {
        scene_object->resource = BattleObjectData_GetLoadState(resource_id);
        scene_object->animation_variant_offset = 0;
    }

    previous_render_state = scene_object->render_state;
    BattleSceneObject_SetAnimationFromComponent(
        scene_object, animation_command & 0xFF, 0,
        object->animation_component);
    BattleSceneObject_SetAnimationMode01(scene_object, model_value);
    model = BattleSceneObject_GetActiveModel(scene_object);
    func_0200940c(model, 0x100);
    model->flags = (model->flags & ~BATTLE_MODEL_FLAG_10) |
                   ((((u8)flag_10) & 1) << BATTLE_MODEL_FLAG_10_SHIFT);
    model->flags &= ~BATTLE_MODEL_FLAG_11;

    if (render_state != -1) {
        scene_object->render_state = render_state;
    } else {
        scene_object->render_state = previous_render_state;
    }

    if (synchronize_animation_layer) {
        int phase = (object->flags << 22) >> 29;
        int divisor = func_02009224(model, -1);
        int animation_offset = phase % divisor;
        BattleModel *getter_model;
        BattleModelVTable *getter_vtable;
        int saved_animation_offset;
        int animation_id;

        /* Preserve the original virtual-call setup and live remainder. */
        asm {
            mov getter_model, model
            ldr getter_vtable, [getter_model]
            mov saved_animation_offset, animation_offset
            ldr getter_vtable, [getter_vtable, #0x3c]
        }
        animation_id =
            ((int (*)(BattleModel *))getter_vtable)(getter_model);
        model->set_primary_animation(animation_id & 0xFF,
                                     (s16)saved_animation_offset, 1);
    }
    model->flags |= BATTLE_MODEL_FLAG_09;
}

int Overlay17Attack_AreObjectStatesIdle(Overlay17AttackObject *objects,
                                        int object_count) {
    int object_index = 0;

    if (object_count > 0) {
        do {
            if (objects->flag_bits.state != 0) {
                return 0;
            }
            ++object_index;
            ++objects;
        } while (object_index < object_count);
    }
    return 1;
}
