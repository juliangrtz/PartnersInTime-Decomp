#include "field_party_internal.h"
/* Baby Spin entry, hovering, gliding, landing and cancellation were observed
 * with story save 83. The alternate state-68 and restore paths retain neutral
 * names until their gameplay behavior has been observed. */
extern "C" {
void func_ov000_02092770(FieldPartyController *, FieldPartyEntity *, int, int, int, int);
void func_ov000_0208be88(FieldPartyController *, FieldPartyEntity *);

void FieldParty_BeginBabySpin(FieldPartyController *party, int member)
{
    FieldPartyEntity *entity;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->unknown_055 = -1;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->state_bits.unknown_01 = 0;
    party->members[member]->entity.unknown_3c8 = 0;
    party->members[member]->entity.unknown_3c8 |= 0x100;
    party->members[member]->entity.field_state_flag_bits.track_ground = 0;
    party->members[member]->state_record->velocity = 4096;
    party->members[member]->state_record->acceleration = 409;
    entity = party->members[member];
    if (entity->linked_member->bits.movement_mode != 7)
        entity->state_record->target_z = entity->entity.position_z + 385024;
    else
        entity->state_record->target_z = entity->linked_member->state_record->target_z;
    party->members[member]->state_record->angle =
        party->members[member]->entity.base_state_flag_bits.facing_direction << 12;
    party->members[member]->state_record->angular_speed = 1228;
    party->members[member]->state_record->unknown_10 = 204;
    party->members[member]->state_record->flags.facing_direction =
        (u16)party->members[member]->entity.base_state_flag_bits.facing_direction;
    if (!party->members[member]->presentation.behavior_saved) {
        party->members[member]->presentation.saved_behavior =
            (u16)party->members[member]->entity.saved_presentation_flag_bits.behavior_mode;
        party->members[member]->presentation.behavior_saved = 1;
    }
    party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->members[member]->entity.locomotion_state = 59;
    party->members[member]->bits.movement_mode = 7;
    FieldParty_BindActionResource(party, party->members[member], member + 33, 256, 0);
    if (party->members[member]->linked_member->bits.movement_mode != 7) {
        party->members[member]->linked_member->entity.locomotion_state = 60;
        party->members[member]->linked_member->bits.movement_mode = 7;
        party->members[member]->linked_member->state_record->linked_member_delay = 4;
    }
    party->leader->presentation.unknown_15 = 0;
    party->follower->presentation.unknown_15 = 0;
    func_ov000_02092770(party, party->members[member], 2, 4, 1, 1);
    GameAudio_PlayEffectDelayed(220, 0, -1);
}
void FieldParty_HoldBabySpin(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 1;
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    func_ov000_0209ce44(party, 0, 9);
    func_ov000_0209ce44(party, 1, 9);
    party->leader->entity.locomotion_state = 62;
    party->follower->entity.locomotion_state = 62;
}
void FieldParty_GlideBabySpin(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 1;
    party->leader->entity.field_state_flag_bits.track_ground = 1;
    party->follower->entity.field_state_flag_bits.track_ground = 1;
    FieldEntity_SetLocomotionParameters(&party->leader->entity, 8192, 0, 8192, 0, 0, 8192);
    FieldEntity_SetLocomotionParameters(&party->follower->entity, 8192, 0, 8192, 0, 0, 8192);
    FieldVertical_Start(&party->leader->entity, -2048, 0, -1);
    FieldVertical_Start(&party->follower->entity, -2048, 0, -1);
    FieldParty_InitializeFollowing(party, 0);
    party->leader->entity.locomotion_state = 63;
    party->follower->entity.locomotion_state = 63;
}
void FieldParty_CancelBabySpin(FieldPartyController *party)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    FieldVertical_Start(&party->leader->entity, 0, 1076, -1);
    FieldVertical_Start(&party->follower->entity, 0, 1076, -1);
    party->leader->entity.locomotion_state = 64;
    party->follower->entity.locomotion_state = 64;
}
void FieldParty_LandBabySpin(FieldPartyController *party, int member)
{
    int other;
    party->flags.previous_field_screen = party->flags.field_screen;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    FieldVertical_Stop(&party->members[member]->entity);
    other = member ^ 1;
    party->members[member]->entity.field_state_flag_bits.track_ground = 0;
    FieldLinear3D_StartFollowing(&party->members[member]->entity, &party->members[other]->entity, 0, 0, 0, 0,
                                 63, 63, 1, &party->members[member]->movement);
    party->members[member]->entity.locomotion_state = 66;
    func_ov000_02092e24(party, party->members[other]);
    FieldEntity_SetFacingDirection(&party->members[other]->entity, 0,
                                   party->members[other]->state_record->flags.facing_direction, 1);
    party->members[other]->entity.locomotion_state = 67;
}
void FieldParty_ResumeState62Or74(FieldPartyController *party, int preserve_state)
{
    int member;
    if (!preserve_state) {
        party->flags.unknown_14 = 0;
        party->flags.unknown_06 = 0;
        party->flags.unknown_10_13 = 0;
        party->leader->entity.movement_speed = 0;
        party->follower->entity.movement_speed = 0;
        party->state_bits.unknown_01 = 0;
        func_ov000_0209ce18(party);
        party->state.unknown_00 = 1;
        party->state.unknown_01 = 1;
        for (member = 0; member < 2; member++) {
            party->members[member]->entity.unknown_3c8 = 0;
            party->members[member]->entity.unknown_3c8 |= 0x100;
            party->members[member]->entity.field_state_flag_bits.track_ground = 0;
            party->members[member]->state_record->angle =
                party->members[member]->entity.base_state_flag_bits.facing_direction << 12;
            party->members[member]->state_record->angular_speed = 1228;
            party->members[member]->state_record->unknown_10 = 204;
            party->members[member]->state_record->flags.facing_direction =
                (u16)party->members[member]->entity.base_state_flag_bits.facing_direction;
            if (party->unknown_055 == -1)
                party->members[member]->entity.locomotion_state = 62;
            else {
                func_ov000_0208be88(party, party->members[member]);
                party->members[member]->entity.locomotion_state = 74;
            }
            party->members[member]->bits.movement_mode = 7;
            party->members[member]->entity.base.map_locomotion_state();
        }
    }
    for (member = 0; member < 2; member++) {
        FieldParty_BindActionResource(party, party->members[member], member + 33, 256, 0);
        if (!party->members[member]->presentation.behavior_saved) {
            party->members[member]->presentation.saved_behavior =
                (u16)party->members[member]->entity.saved_presentation_flag_bits.behavior_mode;
            party->members[member]->presentation.behavior_saved = 1;
        }
        party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 1;
        if (VM_ReadVariable(8196, 0, 0))
            func_ov000_02092770(party, party->members[member], 2, 4, 1, 1);
    }
    if (VM_ReadVariable(8196, 0, 0)) {
        /* The native range test uses a wrapping 16-bit offset from state 73. */
        if ((u16)(party->leader->entity.locomotion_state + (u16)-73) <= 1) {
            func_ov000_0209ce44(party, 0, 30);
            func_ov000_0209ce44(party, 1, 30);
        } else {
            func_ov000_0209ce44(party, 0, 9);
            func_ov000_0209ce44(party, 1, 9);
        }
    }
}
void FieldParty_BeginState68(FieldPartyController *party, int member)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->state_bits.unknown_01 = 0;
    party->members[member]->state_record->angle =
        party->members[member]->entity.base_state_flag_bits.facing_direction << 12;
    party->members[member]->state_record->angular_speed = 1228;
    party->members[member]->state_record->unknown_10 = 204;
    party->members[member]->state_record->flags.facing_direction =
        (u16)party->members[member]->entity.base_state_flag_bits.facing_direction;
    if (!party->members[member]->presentation.behavior_saved) {
        party->members[member]->presentation.saved_behavior =
            (u16)party->members[member]->entity.saved_presentation_flag_bits.behavior_mode;
        party->members[member]->presentation.behavior_saved = 1;
    }
    party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->members[member]->entity.locomotion_state = 68;
    party->members[member]->bits.movement_mode = 7;
    FieldParty_BindActionResource(party, party->members[member], member + 33, 256, 0);
    if (party->members[member]->linked_member->bits.movement_mode != 7) {
        party->members[member]->linked_member->entity.locomotion_state = 69;
        party->members[member]->linked_member->bits.movement_mode = 7;
        party->members[member]->linked_member->state_record->linked_member_delay = 4;
    }
    party->leader->presentation.unknown_15 = 0;
    party->follower->presentation.unknown_15 = 0;
    GameAudio_PlayEffectDelayed(220, 0, -1);
}
}
