#include <game/field_entity_motion.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_presentation.h>
extern "C" {
extern void func_ov000_020a401c(FieldRuntimeEntity *, int);
extern void func_ov000_020a3e38(FieldRuntimeEntity *, int);

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
            func_ov000_020a401c(entity, 0);
            func_ov000_020a3e38(entity, 0);
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
