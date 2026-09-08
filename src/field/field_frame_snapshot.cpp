#include <game/field_entity_motion.h>
extern "C" {

void FieldEntity2D_BeginFrame(FieldRuntimeEntity *entity)
{
    entity->previous_position_x = entity->position_x;
    entity->previous_position_y = entity->position_y;
    entity->previous_movement_direction = entity->movement_direction;
    entity->base_state_flag_bits.previous_facing_direction = entity->base_state_flag_bits.facing_direction;
    entity->previous_locomotion_state = entity->locomotion_state;
    entity->previous_locomotion_category = entity->locomotion_category;
    entity->movement_velocity_x = entity->movement_speed;
    entity->unknown_134 = entity->movement_velocity_y;
    entity->previous_frame_delta_x = entity->frame_delta_x;
    entity->previous_frame_delta_y = entity->frame_delta_y;
    entity->frame_delta_x = 0;
    entity->frame_delta_y = 0;
    entity->unknown_1ae[0] = entity->unknown_1a6[0];
    entity->unknown_1ae[1] = entity->unknown_1a6[1];
    entity->unknown_1ae[2] = entity->unknown_1a6[2];
    entity->unknown_1ae[3] = entity->unknown_1a6[3];
}
}
