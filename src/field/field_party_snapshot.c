#include <game/field_party_snapshot.h>

void FieldPartyEntity_SaveSnapshot(FieldPartyEntity *party, FieldPartySnapshot *snapshot)
{
    snapshot->flags.active = party->entity.base.property_00a_bits.property_00a_flag_00;
    snapshot->flags.visibility_07 = party->entity.base.visibility_bits.unknown_07;
    snapshot->flags.presentation_31 = party->entity.saved_presentation_flag_bits.unknown_31;
    snapshot->flags.animation_wait = party->entity.base_state_flag_bits.animation_wait_enabled;
    snapshot->flags.facing = party->entity.base_state_flag_bits.facing_direction;
    snapshot->flags.blink_mode = party->entity.base_state_flag_bits.blink_mode;
    snapshot->flags.blink_paused = party->entity.base_state_flag_bits.blink_paused;
    snapshot->flags.blink_phase = party->entity.base_state_flag_bits.blink_phase;
    snapshot->flags.blink_frames = party->entity.base_state_flag_bits.blink_frames;
    snapshot->flags.blink_offset = party->entity.base_state_flag_bits.blink_offset;
    snapshot->flags.movement_mode = party->bits.movement_mode;
    snapshot->flags.presentation_05 = party->presentation.unknown_05;
    snapshot->contacts.resource_index = party->presentation.resource_index;
    snapshot->contacts.contact_mask_a = party->entity.unknown_3a0_bits.contact_mask_a;
    snapshot->contacts.previous_contact_mask_a = party->entity.unknown_3a0_bits.previous_contact_mask_a;
    snapshot->contacts.contact_mask_b = party->entity.unknown_3a0_bits.contact_mask_b;
    snapshot->contacts.previous_contact_mask_b = party->entity.unknown_3a0_bits.previous_contact_mask_b;
    snapshot->runtime.contact_mask_b = party->entity.runtime_flag_bits.contact_mask_b;
    snapshot->runtime.previous_contact_mask_b = party->entity.runtime_flag_bits.previous_contact_mask_b;
    snapshot->runtime.saved_animation_speed = party->saved_animation_speed;
    snapshot->runtime.presentation_00 = party->presentation.unknown_00;
    snapshot->runtime.presentation_14 = party->presentation.unknown_14;
    snapshot->runtime.presentation_15 = party->presentation.unknown_15;
    snapshot->runtime.track_ground = party->entity.field_state_flag_bits.track_ground;
    snapshot->extra.runtime_21 = party->entity.runtime_flag_bits.unknown_21;
    snapshot->motion_flags = party->entity.unknown_3c8;
    snapshot->presentation.behavior_mode = party->entity.saved_presentation_flag_bits.behavior_mode;
    snapshot->presentation.saved_behavior_mode =
        party->entity.saved_presentation_flag_bits.saved_behavior_mode;
    snapshot->presentation.has_saved_behavior = party->entity.saved_presentation_flag_bits.has_saved_behavior;
    snapshot->presentation.saved_behavior = party->presentation.saved_behavior;
    snapshot->presentation.behavior_saved = party->presentation.behavior_saved;
    snapshot->presentation.saved_collision = party->bytes.saved_collision;
    snapshot->presentation.collision_saved = party->bytes.collision_saved;
    snapshot->presentation.saved_collision_faces = party->bits.saved_collision_faces;
    snapshot->presentation.collision_faces_saved = party->bits.collision_faces_saved;
    snapshot->presentation.collision_23 = party->entity.collision_extra_bits.unknown_23;
    snapshot->locomotion.starting_speed = party->entity.locomotion.starting_speed;
    snapshot->locomotion.acceleration = party->entity.locomotion.acceleration;
    snapshot->locomotion.maximum_speed = party->entity.locomotion.maximum_speed;
    snapshot->locomotion.deceleration = party->entity.locomotion.deceleration;
    snapshot->locomotion.reverse_deceleration = party->entity.locomotion.reverse_deceleration;
    snapshot->locomotion.turn_speed_limit = party->entity.locomotion.turn_speed_limit;
    snapshot->initial_locomotion.starting_speed = party->entity.initial_locomotion.starting_speed;
    snapshot->initial_locomotion.acceleration = party->entity.initial_locomotion.acceleration;
    snapshot->initial_locomotion.maximum_speed = party->entity.initial_locomotion.maximum_speed;
    snapshot->initial_locomotion.deceleration = party->entity.initial_locomotion.deceleration;
    snapshot->initial_locomotion.reverse_deceleration = party->entity.initial_locomotion.reverse_deceleration;
    snapshot->initial_locomotion.turn_speed_limit = party->entity.initial_locomotion.turn_speed_limit;
    snapshot->direction = party->entity.movement_direction;
    snapshot->animation_speed = party->entity.animation_speed;
    snapshot->animation_id = party->entity.animation_id;
    snapshot->support_index = party->entity.support_entity_index;
    snapshot->previous_support_index = party->entity.previous_support_entity_index;
    snapshot->linked_index =
        party->entity.unknown_500 ? ((FieldEntity *)party->entity.unknown_500)->index : -1;
    snapshot->unknown_52 = party->unknown_596;
    snapshot->unknown_53 = party->unknown_598;
    snapshot->locomotion_state = party->entity.locomotion_state;
    snapshot->unknown_56 = party->entity.unknown_370;
    snapshot->default_vertical_launch_velocity = party->entity.default_vertical_launch_velocity;
    snapshot->default_gravity = party->entity.default_gravity;
    snapshot->terminal_fall_velocity = party->entity.terminal_fall_velocity;
    snapshot->initial_vertical_launch_velocity = party->entity.initial_vertical_launch_velocity;
    snapshot->initial_gravity = party->entity.initial_gravity;
    snapshot->initial_terminal_fall_velocity = party->entity.initial_terminal_fall_velocity;
    snapshot->x = party->entity.position_x;
    snapshot->y = party->entity.position_y;
    snapshot->z = party->entity.position_z;
    snapshot->relative_height = party->entity.relative_height;
    snapshot->support_clearance = party->entity.support_clearance;
    snapshot->current_contacts.contact_active = party->entity.unknown_3a0_bits.contact_active;
    snapshot->current_contacts.previous_contact_active =
        party->entity.unknown_3a0_bits.previous_contact_active;
    snapshot->current_contacts.collision = party->entity.collision_flag_bytes.current;
    snapshot->current_contacts.alternate_collision_faces =
        party->entity.runtime_flag_bits.alternate_collision_faces;
}

FieldPartyEntity *FieldPartyEntity_RestoreSnapshot(FieldPartyEntity *party,
                                                   const FieldPartySnapshot *snapshot,
                                                   FieldRuntimeEntity **entities)
{
    party->entity.base.property_00a_bits.property_00a_flag_00 = snapshot->flags.active;
    party->entity.base.visibility_bits.unknown_07 = snapshot->flags.visibility_07;
    party->entity.saved_presentation_flag_bits.unknown_31 = snapshot->flags.presentation_31;
    party->entity.base_state_flag_bits.animation_wait_enabled = snapshot->flags.animation_wait;
    party->entity.base_state_flag_bits.facing_direction = snapshot->flags.facing;
    party->entity.base_state_flag_bits.blink_mode = snapshot->flags.blink_mode;
    party->entity.base_state_flag_bits.blink_paused = snapshot->flags.blink_paused;
    party->entity.base_state_flag_bits.blink_phase = snapshot->flags.blink_phase;
    party->entity.base_state_flag_bits.blink_frames = snapshot->flags.blink_frames;
    party->entity.base_state_flag_bits.blink_offset = snapshot->flags.blink_offset;
    party->bits.movement_mode = snapshot->flags.movement_mode;
    party->presentation.unknown_05 = snapshot->flags.presentation_05;
    party->presentation.resource_index = snapshot->contacts.resource_index;
    party->entity.unknown_3a0_bits.contact_mask_a = snapshot->contacts.contact_mask_a;
    party->entity.unknown_3a0_bits.previous_contact_mask_a = snapshot->contacts.previous_contact_mask_a;
    party->entity.unknown_3a0_bits.contact_mask_b = snapshot->contacts.contact_mask_b;
    party->entity.unknown_3a0_bits.previous_contact_mask_b = snapshot->contacts.previous_contact_mask_b;
    party->entity.runtime_flag_bits.contact_mask_b = snapshot->runtime.contact_mask_b;
    party->entity.runtime_flag_bits.previous_contact_mask_b = snapshot->runtime.previous_contact_mask_b;
    party->saved_animation_speed = snapshot->runtime.saved_animation_speed;
    party->presentation.unknown_00 = snapshot->runtime.presentation_00;
    party->presentation.unknown_14 = snapshot->runtime.presentation_14;
    party->presentation.unknown_15 = snapshot->runtime.presentation_15;
    party->entity.field_state_flag_bits.track_ground = snapshot->runtime.track_ground;
    party->entity.runtime_flag_bits.unknown_21 = snapshot->extra.runtime_21;
    party->entity.unknown_3c8 = snapshot->motion_flags;
    party->entity.saved_presentation_flag_bits.behavior_mode = snapshot->presentation.behavior_mode;
    party->entity.saved_presentation_flag_bits.saved_behavior_mode =
        snapshot->presentation.saved_behavior_mode;
    party->entity.saved_presentation_flag_bits.has_saved_behavior = snapshot->presentation.has_saved_behavior;
    party->presentation.saved_behavior = snapshot->presentation.saved_behavior;
    party->presentation.behavior_saved = snapshot->presentation.behavior_saved;
    party->bytes.saved_collision = snapshot->presentation.saved_collision;
    party->bytes.collision_saved = snapshot->presentation.collision_saved;
    party->bits.saved_collision_faces = snapshot->presentation.saved_collision_faces;
    party->bits.collision_faces_saved = snapshot->presentation.collision_faces_saved;
    party->entity.collision_extra_bits.unknown_23 = snapshot->presentation.collision_23;
    party->entity.locomotion.starting_speed = snapshot->locomotion.starting_speed;
    party->entity.locomotion.acceleration = snapshot->locomotion.acceleration;
    party->entity.locomotion.maximum_speed = snapshot->locomotion.maximum_speed;
    party->entity.locomotion.deceleration = snapshot->locomotion.deceleration;
    party->entity.locomotion.reverse_deceleration = snapshot->locomotion.reverse_deceleration;
    party->entity.locomotion.turn_speed_limit = snapshot->locomotion.turn_speed_limit;
    party->entity.initial_locomotion.starting_speed = snapshot->initial_locomotion.starting_speed;
    party->entity.initial_locomotion.acceleration = snapshot->initial_locomotion.acceleration;
    party->entity.initial_locomotion.maximum_speed = snapshot->initial_locomotion.maximum_speed;
    party->entity.initial_locomotion.deceleration = snapshot->initial_locomotion.deceleration;
    party->entity.initial_locomotion.reverse_deceleration = snapshot->initial_locomotion.reverse_deceleration;
    party->entity.initial_locomotion.turn_speed_limit = snapshot->initial_locomotion.turn_speed_limit;
    party->entity.movement_direction = snapshot->direction;
    party->entity.animation_speed = snapshot->animation_speed;
    party->entity.animation_id = snapshot->animation_id;
    party->entity.support_entity_index = snapshot->support_index;
    party->entity.previous_support_entity_index = snapshot->previous_support_index;
    party->entity.unknown_500 = snapshot->linked_index != -1 ? entities[snapshot->linked_index] : 0;
    party->unknown_596 = snapshot->unknown_52;
    party->unknown_598 = snapshot->unknown_53;
    party->entity.locomotion_state = snapshot->locomotion_state;
    party->entity.unknown_370 = snapshot->unknown_56;
    party->entity.default_vertical_launch_velocity = snapshot->default_vertical_launch_velocity;
    party->entity.default_gravity = snapshot->default_gravity;
    party->entity.terminal_fall_velocity = snapshot->terminal_fall_velocity;
    party->entity.initial_vertical_launch_velocity = snapshot->initial_vertical_launch_velocity;
    party->entity.initial_gravity = snapshot->initial_gravity;
    party->entity.initial_terminal_fall_velocity = snapshot->initial_terminal_fall_velocity;
    party->entity.previous_position_x = snapshot->x;
    party->entity.position_x = party->entity.previous_position_x;
    party->entity.previous_position_y = snapshot->y;
    party->entity.position_y = party->entity.previous_position_y;
    party->entity.previous_position_z = snapshot->z;
    party->entity.position_z = party->entity.previous_position_z;
    party->entity.previous_relative_height = snapshot->relative_height;
    party->entity.relative_height = party->entity.previous_relative_height;
    party->entity.previous_support_clearance = snapshot->support_clearance;
    party->entity.support_clearance = party->entity.previous_support_clearance;
    party->entity.unknown_3a0_bits.contact_active = snapshot->current_contacts.contact_active;
    party->entity.unknown_3a0_bits.previous_contact_active =
        snapshot->current_contacts.previous_contact_active;
    party->entity.collision_flag_bytes.current = snapshot->current_contacts.collision;
    party->entity.runtime_flag_bits.alternate_collision_faces =
        snapshot->current_contacts.alternate_collision_faces;
    return party;
}
