#include "field_party_internal.h"
#include <game/field_blink.h>

extern "C" {
extern const s16 FX_SinCosTable_[];
extern const s8 data_ov000_020c06ec[];
}

static inline void SaveBehavior(FieldPartyController *party, int member)
{
    if (!party->members[member]->presentation.behavior_saved) {
        party->members[member]->presentation.saved_behavior =
            (u16)party->members[member]->entity.saved_presentation_flag_bits.behavior_mode;
        party->members[member]->presentation.behavior_saved = 1;
    }
}
extern "C" void FieldParty_BeginState81(FieldPartyController *party, int airborne)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    GameAudio_StopEffect(312);
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->flags.unknown_06 = 0;
    if (!airborne) {
        FieldLinear3D_Start(
            &party->leader->entity, 0,
            (party->paired->leader->entity.position_x + party->paired->follower->entity.position_x) / 2,
            (party->paired->leader->entity.position_y + party->paired->follower->entity.position_y) / 2, 0,
            party->leader->entity.movement_speed, 0, 0, party->leader->entity.movement_speed / 4, 0, 0, 1,
            &party->leader->movement);
        FieldVertical_Start(&party->leader->entity, 21496, 1296, 0);
    }
    party->members[0]->state_record->resources.saved_movement_speed = party->leader->entity.movement_speed;
    party->members[0]->state_record->resources.flags.saved_movement = party->flags.unknown_10_13;
    party->members[0]->state_record->resources.flags.unknown_00 = 0;
    party->leader->entity.movement_speed = 0;
    party->flags.unknown_10_13 = 0;
    SaveBehavior(party, 0);
    party->leader->entity.saved_presentation_flag_bits.behavior_mode = 1;
    party->leader->entity.locomotion_state = 81;
    GameAudio_PlayEffectDelayed(224, 0, -1);
}
extern "C" void FieldParty_BeginPairedStates82Or83(FieldPartyController *party, int member_mask, int airborne)
{
    party->flags.previous_field_screen = party->flags.field_screen;
    for (int member = 0; member < 2; ++member) {
        if (member_mask & (1 << member)) {
            FieldEntity_SetFacingDirection(
                &party->members[member]->entity, 0,
                party->paired->leader->entity.base_state_flag_bits.facing_direction ^ 4, 1);
            if (!airborne) {
                SaveBehavior(party, member);
                party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 1;
                FieldParty_BindActionResource(party, party->members[member], member + 37, 256, 1);
                party->members[member]->entity.render_object->state_flag_bits.behavior_state = 1;
                party->members[member]->entity.render_object->state_flag_bits.animation_suppressed = 0;
                party->members[member]->entity.locomotion_state = 82;
            } else {
                SaveBehavior(party, member);
                party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 0;
                party->members[member]->entity.locomotion_state = 83;
            }
        } else {
            SaveBehavior(party, member);
            party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 0;
            party->members[member]->entity.locomotion_state = 83;
        }
    }
    party->members[0]->state_record->resources.flags.unknown_00 = 0;
    party->state_bits.unknown_01 = 0;
    party->state_bits.unknown_03_04 = 0;
    FieldParty_RefreshActions(party);
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->flags.unknown_04 = 0;
    party->flags.unknown_14 = 0;
    party->flags.unknown_06 = 0;
}
extern "C" void FieldParty_BeginState8(FieldPartyController *party)
{
    int member = 0;
    party->flags.previous_field_screen = party->flags.field_screen;
    do {
        party->members[member]->entity.locomotion_state = 0;
        if (party->members[member]->presentation.behavior_saved) {
            party->members[member]->entity.saved_presentation_flag_bits.behavior_mode =
                party->members[member]->presentation.saved_behavior;
            party->members[member]->presentation.behavior_saved = 0;
        } else {
            party->members[member]->entity.saved_presentation_flag_bits.behavior_mode = 3;
        }
        FieldParty_BindActionResource(party, party->members[member], member + 35, 256, 0);
        party->members[member]->bits.movement_mode = 8;
        FieldEntity_SetLocomotionParameters(&party->members[member]->entity, 5734, 0, 5734, -5734, 0, 5734);
        FieldEntity_SetVerticalParameters(&party->members[member]->entity, 15792, 800, 0);
        party->members[member]->entity.field_state_flag_bits.body_collision_enabled = 1;
        party->members[member]->entity.unknown_3c8 = 0;
        party->members[member]->entity.unknown_3c8 |= 8;
        party->members[member]->state_record->resources.flags.unknown_20_21 = 0;
        party->members[member]->state_record->resources.flags.unknown_01_14 = 420;
    } while (++member < 2);
    if (party->flags.movement_active)
        FieldParty_InitializeFollowing(party, 65536);
    party->state.unknown_00 = 1;
    party->state.unknown_01 = 1;
    party->flags.unknown_04 = 1;
    party->flags.unknown_06 = 1;
}
extern "C" void FieldParty_UpdateHover(FieldPartyController *party, int member)
{
    ++party->members[member]->state_record->resources.bob_phase;
    party->members[member]->state_record->resources.bob_phase &= 63;
    party->members[member]->entity.position_z -=
        FX_SinCosTable_[((party->members[member]->state_record->resources.bob_phase << 10) >> 4) * 2] / 2;
}
extern "C" void FieldParty_BeginRecoveryBlink(FieldPartyController *party, int member)
{
    if (party->members[member]->state_record->resources.flags.unknown_20_21 == 1) {
        party->members[member]->state_record->resources.flags.unknown_20_21 = 0;
        party->members[member]->state_record->resources.flags.unknown_01_14 = 0;
        FieldBlink_Stop(&party->members[member]->entity, 1);
    }
    party->flags.previous_field_screen = party->flags.field_screen;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->flags.unknown_04 = 0;
    party->flags.unknown_06 = 0;
    party->members[member]->state_record->resources.resource_18 = party->resources + 24 * (member + 35);
    party->members[member]->state_record->resources.resource_1c = party->resources + 24 * (member + 16);
    party->members[member]->state_record->resources.flags.update_bounds = 1;
    party->members[member]->entity.base.start_blink(
        3, data_ov000_020c06ec, 24, (FieldEntityVisibilityCallback)FieldPartyEntity_BindStateResource18,
        (FieldEntityVisibilityCallback)FieldPartyEntity_BindStateResource1C);
    party->members[member]->entity.field_state_flag_bits.vertical_motion_paused = 1;
    party->members[member]->entity.locomotion_state = 88;
    GameAudio_PlayEffectDelayed(227, 0, -1);
}
extern "C" void FieldParty_FinishRecoveryBlink(FieldPartyController *party, int member)
{
    func_ov000_02092e24(party, party->members[member]);
    party->members[member]->entity.locomotion_state = 0;
    party->members[member]->entity.base.map_locomotion_state();
    party->members[member]->entity.unknown_3c8 = 0;
    party->members[member]->entity.unknown_3c8_bits.unknown_00 = 1;
    party->members[member]->entity.field_state_flag_bits.vertical_motion_paused = 0;
    party->members[member]->entity.base.reset_motion_parameters();
    party->members[member]->state_record->resources.flags.unknown_20_21 = 2;
}
