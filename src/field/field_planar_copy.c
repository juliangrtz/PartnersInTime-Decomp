#include <game/field_entity.h>
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
FieldRuntimeEntity *FieldEntity_CopyPlanarState(FieldRuntimeEntity *entity, const FieldRuntimeEntity *source)
{
    entity->position_x = source->position_x;
    entity->position_y = source->position_y;
    entity->previous_position_x = source->previous_position_x;
    entity->previous_position_y = source->previous_position_y;
    entity->bounds_index = source->bounds_index;
    entity->interaction_min_x = source->interaction_min_x;
    entity->interaction_min_y = source->interaction_min_y;
    entity->interaction_max_x = source->interaction_max_x;
    entity->interaction_max_y = source->interaction_max_y;
    entity->interaction_vertical_extent = source->interaction_vertical_extent;
    entity->movement_speed = source->movement_speed;
    entity->movement_velocity_x = source->movement_velocity_x;
    entity->movement_velocity_y = source->movement_velocity_y;
    entity->unknown_134 = source->unknown_134;
    entity->locomotion.starting_speed = source->locomotion.starting_speed;
    entity->locomotion.acceleration = source->locomotion.acceleration;
    entity->locomotion.maximum_speed = source->locomotion.maximum_speed;
    entity->locomotion.deceleration = source->locomotion.deceleration;
    entity->locomotion.reverse_deceleration = source->locomotion.reverse_deceleration;
    entity->locomotion.turn_speed_limit = source->locomotion.turn_speed_limit;
    entity->initial_locomotion.starting_speed = source->initial_locomotion.starting_speed;
    entity->initial_locomotion.acceleration = source->initial_locomotion.acceleration;
    entity->initial_locomotion.maximum_speed = source->initial_locomotion.maximum_speed;
    entity->initial_locomotion.deceleration = source->initial_locomotion.deceleration;
    entity->initial_locomotion.reverse_deceleration = source->initial_locomotion.reverse_deceleration;
    entity->initial_locomotion.turn_speed_limit = source->initial_locomotion.turn_speed_limit;
    entity->frame_delta_x = source->frame_delta_x;
    entity->frame_delta_y = source->frame_delta_y;
    entity->previous_frame_delta_x = source->previous_frame_delta_x;
    entity->previous_frame_delta_y = source->previous_frame_delta_y;
    entity->base_state_flag_bits.animation_wait_enabled = source->base_state_flag_bits.animation_wait_enabled;
    entity->base_state_flag_bits.unknown_01 = source->base_state_flag_bits.unknown_01;
    entity->base_state_flag_bits.facing_direction = source->base_state_flag_bits.facing_direction;
    entity->base_state_flag_bits.previous_facing_direction =
        source->base_state_flag_bits.previous_facing_direction;
    entity->base_state_flag_bits.retain_offscreen_contact =
        source->base_state_flag_bits.retain_offscreen_contact;
    entity->base_state_flag_bits.unknown_10 = source->base_state_flag_bits.unknown_10;
    entity->base_state_flag_bits.unknown_11 = source->base_state_flag_bits.unknown_11;
    entity->base_state_flag_bits.reserved_state = source->base_state_flag_bits.reserved_state;
    entity->base_state_flag_bits.blink_mode = source->base_state_flag_bits.blink_mode;
    entity->base_state_flag_bits.blink_paused = source->base_state_flag_bits.blink_paused;
    entity->base_state_flag_bits.blink_phase = source->base_state_flag_bits.blink_phase;
    entity->base_state_flag_bits.blink_frames = source->base_state_flag_bits.blink_frames;
    entity->base_state_flag_bits.blink_offset = source->base_state_flag_bits.blink_offset;
    entity->blink_durations = source->blink_durations;
    entity->blink_first_phase = source->blink_first_phase;
    entity->blink_second_phase = source->blink_second_phase;
    entity->saved_presentation_flag_bits.resource_set = source->saved_presentation_flag_bits.resource_set;
    entity->saved_presentation_flag_bits.palette_slot = source->saved_presentation_flag_bits.palette_slot;
    entity->saved_presentation_flag_bits.behavior_mode = source->saved_presentation_flag_bits.behavior_mode;
    entity->saved_presentation_flag_bits.saved_behavior_mode =
        source->saved_presentation_flag_bits.saved_behavior_mode;
    entity->saved_presentation_flag_bits.has_saved_behavior =
        source->saved_presentation_flag_bits.has_saved_behavior;
    entity->saved_presentation_flag_bits.unknown_14 = source->saved_presentation_flag_bits.unknown_14;
    entity->saved_presentation_flag_bits.unknown_15 = source->saved_presentation_flag_bits.unknown_15;
    entity->saved_presentation_flag_bits.has_saved_resource =
        source->saved_presentation_flag_bits.has_saved_resource;
    entity->saved_presentation_flag_bits.has_saved_palette_profile =
        source->saved_presentation_flag_bits.has_saved_palette_profile;
    entity->saved_presentation_flag_bits.has_saved_resource_animation =
        source->saved_presentation_flag_bits.has_saved_resource_animation;
    entity->saved_presentation_flag_bits.has_saved_animation =
        source->saved_presentation_flag_bits.has_saved_animation;
    entity->saved_presentation_flag_bits.skip_auxiliary_renderer =
        source->saved_presentation_flag_bits.skip_auxiliary_renderer;
    entity->saved_presentation_flag_bits.unknown_31 = source->saved_presentation_flag_bits.unknown_31;
    entity->resource_index = source->resource_index;
    entity->saved_resource_index = source->saved_resource_index;
    entity->presentation_state = source->presentation_state;
    entity->saved_presentation_state = source->saved_presentation_state;
    entity->animation_id = source->animation_id;
    entity->saved_animation_id = source->saved_animation_id;
    entity->saved_model_animation = source->saved_model_animation;
    entity->saved_animation_frame = source->saved_animation_frame;
    entity->animation_speed = source->animation_speed;
    entity->unknown_1a6[0] = source->unknown_1a6[0];
    entity->unknown_1a6[1] = source->unknown_1a6[1];
    entity->unknown_1a6[2] = source->unknown_1a6[2];
    entity->unknown_1a6[3] = source->unknown_1a6[3];
    entity->unknown_1ae[0] = source->unknown_1ae[0];
    entity->unknown_1ae[1] = source->unknown_1ae[1];
    entity->unknown_1ae[2] = source->unknown_1ae[2];
    entity->unknown_1ae[3] = source->unknown_1ae[3];
    entity->unknown_1b8 = source->unknown_1b8;
    entity->unknown_1bc = source->unknown_1bc;
    entity->unknown_1c0 = source->unknown_1c0;
    entity->unknown_1c4 = source->unknown_1c4;
    entity->unknown_1c6 = source->unknown_1c6;
    entity->unknown_1c8 = source->unknown_1c8;
    entity->unknown_1ca = source->unknown_1ca;
    entity->unknown_1cc = source->unknown_1cc;
    entity->unknown_1d0 = source->unknown_1d0;
    entity->unknown_1d4 = source->unknown_1d4;
    entity->unknown_1d6 = source->unknown_1d6;
    entity->unknown_1d8 = source->unknown_1d8;
    if (&source->linear_controller < &entity->linear_controller)
        func_0202cd2c(&source->linear_controller, &entity->linear_controller,
                      sizeof(entity->linear_controller));
    else
        func_0202cc58(&source->linear_controller, &entity->linear_controller,
                      sizeof(entity->linear_controller));
    if (&source->orbit_controller < &entity->orbit_controller)
        func_0202cd2c(&source->orbit_controller, &entity->orbit_controller, sizeof(entity->orbit_controller));
    else
        func_0202cc58(&source->orbit_controller, &entity->orbit_controller, sizeof(entity->orbit_controller));
    if (&source->transform_state < &entity->transform_state)
        func_0202cd2c(&source->transform_state, &entity->transform_state, sizeof(entity->transform_state));
    else
        func_0202cc58(&source->transform_state, &entity->transform_state, sizeof(entity->transform_state));
    FieldEntity_CopyState(&entity->base, &source->base);
    return entity;
}
