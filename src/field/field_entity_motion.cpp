extern "C" {

/*
 * 2D entity motion (overlay 0, 0x020A6690-0x020A695C).
 *
 * One frame of a 2D entity's movement: the locomotion state is updated and
 * mapped to an animation, the accumulated motion is applied, and the facing
 * follows.
 */

#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
extern const u8 data_ov000_020c0b04[];
}
#include <game/field_entity_motion.h>
extern "C" {

void FieldEntity2D_UpdateLocomotionState(FieldRuntimeEntity *entity)
{
    if (entity->base.property_00a_bits.property_00a_flag_00) {
        if (entity->frame_delta_x || entity->frame_delta_y)
            entity->locomotion_state = 1;
        else
            entity->locomotion_state = 0;
        entity->base.map_locomotion_state();
    }
}

void FieldEntity2D_MapLocomotionState(FieldRuntimeEntity *entity)
{
    entity->locomotion_category = data_ov000_020c0b04[entity->locomotion_state];
}

void FieldEntity2D_AccumulateMotion(FieldRuntimeEntity *entity, fx32 x, fx32 y)
{
    entity->frame_delta_x += x;
    entity->frame_delta_y += y;
    if (entity->frame_delta_x || entity->frame_delta_y) {
        entity->movement_direction = FX_Atan2Idx(entity->frame_delta_y, entity->frame_delta_x) + 0x4000;
        if (entity->saved_presentation_flag_bits.behavior_mode >= 3 && (x || y)) {
            entity->base_state_flag_bits.facing_direction =
                (u32)(u16)(FX_Atan2Idx(entity->frame_delta_y, entity->frame_delta_x) + 0x4000) >> 13;
        }
    }
}
}

#include <game/field_transform.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_presentation.h>
extern "C" {

void FieldEntity2D_UpdateMotionAndAnimation(FieldRuntimeEntity *entity)
{
    if (!entity->saved_presentation_flag_bits.unknown_31) {
        entity->base.unknown_78();
        if (entity->base.property_00a_bits.property_00a_flag_00) {
            entity->base.update_linear_movement(0);
            entity->base.update_orbit_movement(0);
            entity->position_x += entity->frame_delta_x;
            entity->position_y += entity->frame_delta_y;
            entity->base.check_linear_completion(0);
            entity->base.advance_orbit_frame(0);
        }
        if (entity->base_state_flag_bits.animation_wait_enabled) {
            FieldTransform_Advance(entity, 0);
            FieldTransform_CheckCompletion(entity, 0);
        }
    }
}
void FieldEntity_UpdateLocomotionState(FieldRuntimeEntity *entity)
{
    entity->base.update_locomotion_state();
}
void FieldEntity2D_SetPosition(FieldRuntimeEntity *entity, fx32 x, fx32 y)
{
    entity->position_x = x;
    entity->position_y = y;
    entity->previous_position_x = x;
    entity->previous_position_y = y;
}
void FieldEntity_SetFacingDirection(FieldRuntimeEntity *entity, int relative, int direction, int refresh)
{
    if (relative == 1)
        direction += entity->base_state_flag_bits.facing_direction;
    entity->base_state_flag_bits.facing_direction = direction;
    if (entity->saved_presentation_flag_bits.behavior_mode >= 3)
        entity->movement_direction = entity->base_state_flag_bits.facing_direction << 13;
    if (refresh)
        entity->base.unknown_64();
}
}
