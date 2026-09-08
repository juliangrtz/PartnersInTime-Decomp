#include "field_party_internal.h"

extern "C" {

void FieldParty_RaiseFollowerToLeader(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->state_bits.unknown_01 = 0;
    party->leader->entity.locomotion_state = 31;
    party->follower->entity.locomotion_state = 28;
    *party->follower->unknown_568 &= ~1;
    func_ov000_02094140(party);
    FieldVertical_StartToHeight(
        &party->follower->entity,
        party->leader->entity.position_z - party->follower->entity.position_z + 24 * 4096, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, 0, 0, 0, 10, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_PlayEffectDelayed(218, 0, -1);
}

void FieldParty_RestartAuxiliary(FieldPartyController *party)
{
    FieldPartyEntity *auxiliary;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 1;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->leader->entity.unknown_3c8 = 0;
    party->leader->entity.unknown_3c8 |= 16;
    party->follower->entity.unknown_3c8 = 0;
    party->follower->entity.unknown_3c8 |= 16;
    party->leader->bits.movement_mode = 1;
    party->follower->bits.movement_mode = 1;
    party->leader->entity.locomotion_state = 0;
    party->follower->entity.locomotion_state = 0;
    func_ov000_02092f30(party, party->leader, 27, 128, 0);
    FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
    func_ov000_0209ce44(party, 0, 9);
    party->follower->entity.base.property_00a |= 0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    auxiliary = party->leader->auxiliary;
    auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 1;
    auxiliary->entity.runtime_flag_bits.sync_horizontal = 0;
    auxiliary->entity.runtime_flag_bits.sync_vertical = 0;
    auxiliary->entity.runtime_flag_bits.unknown_02 = 0;
    auxiliary->entity.runtime_flag_bits.auto_priority_0 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_04 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_06 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_07 = 0;
    auxiliary->entity.field_state_flag_bits.body_collision_enabled = 1;
    auxiliary->auxiliary_bits.unknown_00 = 1;
    auxiliary->auxiliary_bits.unknown_01 = 1;
    auxiliary->auxiliary_motion.target = 0;
    auxiliary->auxiliary_bits.direction_mode = 0;
    auxiliary->auxiliary_bits.unknown_04 = 1;
    auxiliary->auxiliary_bits.unknown_05_06 = 1;
    auxiliary->auxiliary_bits.unknown_07_08 = 2;
    auxiliary->auxiliary_bits.unknown_09 = 1;
    auxiliary->entity.collision_state_flags &= ~0x100000;
    auxiliary->entity.locomotion_state = party->leader->entity.locomotion_state;
    auxiliary->auxiliary_bits.unknown_19 = 0;
    auxiliary->auxiliary_bits.unknown_20_27 = 28;
    auxiliary->auxiliary_bits.unknown_28_31 = 15;
    auxiliary->auxiliary_motion.unknown_53c = 0;
    auxiliary->auxiliary_motion.unknown_540 = 0;
    auxiliary->auxiliary_motion.unknown_544 = 0;
    auxiliary->entity.animation_id = party->leader->entity.base_state_flag_bits.facing_direction;
    func_ov000_020a7010(auxiliary, party->resources + 672, party->leader->entity.unknown_1e8,
                        party->leader->entity.unknown_1ec, 128);
    auxiliary->entity.render_object->state_flag_bits.behavior_state = 0;
    auxiliary->entity.render_object->state_flag_bits.animation_suppressed = 0;
    func_020093b4(auxiliary->entity.render_object, 1);
    FieldParty_UpdateAuxiliaryFacing(party);
}

void FieldParty_ReturnFollowerToLeader(FieldPartyController *party)
{
    fx32 height, x, y;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->leader->bits.movement_mode = 0;
    party->follower->bits.movement_mode = 0;
    party->leader->entity.locomotion_state = 33;
    party->follower->entity.locomotion_state = 32;
    height = party->leader->entity.navigation_vertical_extent;
    func_ov000_02092e24(party, party->leader);
    party->follower->entity.base.property_00a &= ~0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(party->follower->entity.render_object, 1);
    if (!party->follower->presentation.behavior_saved) {
        party->follower->presentation.saved_behavior =
            (u16)party->follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 2;
    FieldEntity_SetFacingDirection(&party->follower->entity, 0,
                                   party->leader->entity.base_state_flag_bits.facing_direction, 1);
    FieldEntity3D_SetPosition(
        &party->follower->entity, party->leader->entity.position_x, party->leader->entity.position_y,
        party->leader->entity.position_z + (height - party->follower->entity.navigation_vertical_extent));
    func_ov000_02093108(party, party->leader, 16 * 4096, &x, &y);
    FieldVertical_StartToHeight(&party->follower->entity, 0, -1, -1);
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, x, y, 0, 10, 0, 0, 1,
                                      &party->follower->movement);
    GameAudio_PlayEffectDelayed(218, 0, -1);
    party->leader->auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 0;
    party->leader->auxiliary->entity.base.set_visible(0);
    party->leader->auxiliary->entity.base.unknown_30();
}

void FieldParty_ReverseFollowerOffset(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    FieldLinear3D_StartTimedFollowing(&party->follower->entity, &party->leader->entity, -party->separation_x,
                                      -party->separation_y, 0, party->follower->movement.elapsed_frames, 0, 0,
                                      1, &party->follower->movement);
    if (!party->follower->presentation.behavior_saved) {
        party->follower->presentation.saved_behavior =
            (u16)party->follower->entity.saved_presentation_flag_bits.behavior_mode;
        party->follower->presentation.behavior_saved = 1;
    }
    party->follower->entity.saved_presentation_flag_bits.behavior_mode = 2;
    party->follower->entity.locomotion_state = 30;
}

void FieldParty_UpdateAuxiliaryFacing(FieldPartyController *party)
{
    party->leader->auxiliary->auxiliary_motion.target = party->leader;
    party->leader->auxiliary->auxiliary_bits.direction_mode =
        data_ov000_020c06a4[party->leader->auxiliary->entity.base_state_flag_bits.facing_direction];
    party->leader->auxiliary->auxiliary_bits.unknown_04 = 1;
    party->leader->auxiliary->auxiliary_bits.unknown_05_06 = 1;
    party->leader->auxiliary->auxiliary_bits.unknown_07_08 = 2;
}

void FieldParty_InitializeAuxiliary(FieldPartyController *party, int preserve_state)
{
    FieldPartyEntity *auxiliary;
    if (!preserve_state) {
        party->flags.unknown_14 = 0;
        party->state_bits.unknown_01 = 0;
        party->state_bits.unknown_03_04 = VM_ReadVariable(8194, 0, 0) | 2;
        func_ov000_0209ce18(party);
        *party->follower->unknown_568 &= ~1;
        party->leader->entity.locomotion_state = 0;
        party->follower->entity.locomotion_state = 0;
        party->leader->bits.movement_mode = 1;
        party->follower->bits.movement_mode = 1;
        party->leader->entity.base.map_locomotion_state();
        FieldEntity_SetLocomotionParameters(&party->leader->entity, 4096, 0, 4096, -4096, 0, 4096);
        party->flags.unknown_04 = 1;
        party->flags.unknown_06 = 1;
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        party->leader->entity.unknown_3c8 = 0;
        party->leader->entity.unknown_3c8 |= 16;
        party->follower->entity.unknown_3c8 = 0;
        party->follower->entity.unknown_3c8 |= 16;
    }
    func_ov000_02092f30(party, party->leader, 27, 128, 0);
    func_ov000_0209ce44(party, 0, 9);
    party->follower->entity.base.property_00a |= 0x80;
    party->follower->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(party->follower->entity.render_object, 0);
    auxiliary = party->leader->auxiliary;
    auxiliary->entity.base.property_00a_bits.property_00a_flag_00 = 1;
    auxiliary->entity.base_state_flag_bits.animation_wait_enabled = 1;
    auxiliary->entity.runtime_flag_bits.sync_horizontal = 0;
    auxiliary->entity.runtime_flag_bits.sync_vertical = 0;
    auxiliary->entity.runtime_flag_bits.unknown_02 = 0;
    auxiliary->entity.runtime_flag_bits.auto_priority_0 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_04 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_06 = 0;
    auxiliary->entity.collision_state_flag_bits.unknown_07 = 0;
    auxiliary->entity.field_state_flag_bits.body_collision_enabled = 1;
    auxiliary->auxiliary_bits.unknown_00 = 1;
    auxiliary->auxiliary_bits.unknown_01 = 1;
    auxiliary->auxiliary_motion.target = 0;
    auxiliary->auxiliary_bits.direction_mode = 0;
    auxiliary->auxiliary_bits.unknown_04 = 1;
    auxiliary->auxiliary_bits.unknown_05_06 = 1;
    auxiliary->auxiliary_bits.unknown_07_08 = 2;
    auxiliary->auxiliary_bits.unknown_09 = 1;
    auxiliary->entity.collision_state_flags &= ~0x100000;
    auxiliary->entity.locomotion_state = party->leader->entity.locomotion_state;
    auxiliary->auxiliary_bits.unknown_19 = 0;
    auxiliary->auxiliary_bits.unknown_20_27 = 28;
    auxiliary->auxiliary_bits.unknown_28_31 = 15;
    auxiliary->auxiliary_motion.unknown_53c = 0;
    auxiliary->auxiliary_motion.unknown_540 = 0;
    auxiliary->auxiliary_motion.unknown_544 = 0;
    auxiliary->entity.animation_id = party->leader->entity.base_state_flag_bits.facing_direction;
    func_ov000_020a7010(auxiliary, party->resources + 672, party->leader->entity.unknown_1e8,
                        party->leader->entity.unknown_1ec, 128);
    auxiliary->entity.render_object->state_flag_bits.behavior_state = 0;
    auxiliary->entity.render_object->state_flag_bits.animation_suppressed = 0;
    func_020093b4(auxiliary->entity.render_object, 1);
    FieldParty_UpdateAuxiliaryFacing(party);
}
}
