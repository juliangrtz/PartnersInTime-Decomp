extern "C" {
#include <nitro/fx.h>
#include <nitro/fx_atan.h>
#include <game/field_geometry.h>
}
#include "field_party_internal.h"

extern "C" void FieldParty_FinishAirborneTransfer(FieldPartyController *party, int member)
{
    FieldPartyEntity *current = party->members[member];
    FieldPartyEntity *paired = party->paired->members[member];
    FieldVertical_Stop(&current->entity);
    current->entity.locomotion_state = 0;
    current->entity.base.visibility_bits.unknown_07 = 1;
    current->entity.saved_presentation_flag_bits.unknown_31 = 1;
    func_020093b4(current->entity.render_object, 0);
    current->entity.runtime_flag_bits.sync_horizontal = 1;
    current->entity.runtime_flag_bits.unknown_08 = 0;
    func_ov000_02092f30(party, paired, member + 39, 256, 0);
}
extern "C" void FieldParty_BeginPiggybackMount(FieldPartyController *party, int member)
{
    FieldPartyEntity *current = party->members[member];
    FieldPartyEntity *paired = party->paired->members[member];
    party->flags.previous_field_screen = party->flags.field_screen;
    if (member == 1 && party->flags.movement_active) {
        FieldPartyEntity *linked = current->linked_member;
        if (linked->entity.locomotion_state == 49) {
            fx32 x, y;
            FieldGeometry_GetDirectionVector(linked->entity.base_state_flag_bits.facing_direction, 327680, &x,
                                             &y);
            u16 angle = FX_Atan2Idx(linked->entity.position_y + y - current->entity.position_y,
                                    linked->entity.position_x + x - current->entity.position_x) +
                        0x4000;
            u16 direction = ((int)angle + 4096) / 8192;
            FieldEntity_SetFacingDirection(&current->entity, 0, direction, 1);
            FieldEntity_SetFacingDirection(&paired->entity, 0, direction, 1);
        }
    }
    party->flags.unknown_06 = 0;
    party->flags.unknown_10_13 = 0;
    party->leader->entity.movement_speed = 0;
    party->follower->entity.movement_speed = 0;
    party->state.unknown_00 = 0;
    party->state.unknown_01 = 0;
    party->paired->state.unknown_00 = 0;
    party->paired->state.unknown_01 = 0;
    party->flags.unknown_04 = 0;
    party->paired->flags.unknown_04 = 0;
    current->presentation.unknown_15 = 0;
    paired->presentation.unknown_15 = 0;
    if (!current->presentation.behavior_saved) {
        current->presentation.saved_behavior = current->entity.saved_presentation_flag_bits.behavior_mode;
        current->presentation.behavior_saved = 1;
    }
    current->entity.saved_presentation_flag_bits.behavior_mode = 1;
    current->entity.locomotion_state = 49;
    paired->entity.locomotion_state = 51;
    current->bits.movement_mode = 0;
    paired->bits.movement_mode = 0;
    party->paired->state_bits.movement_mode = 15;
    func_ov000_02092f30(party, current, member + 43, 256, 0);
    current->entity.render_object->state_flag_bits.behavior_state = 1;
    GameAudio_PlayEffectDelayed(222, 0, -1);
}
extern "C" void FieldParty_RestorePairedMember(FieldPartyController *party, int member)
{
    FieldPartyEntity *current = party->members[member];
    FieldPartyEntity *paired = party->paired->members[member];
    party->flags.previous_field_screen = party->flags.field_screen;
    FieldEntity3D_SetPosition(&paired->entity, current->entity.position_x, current->entity.position_y,
                              current->entity.position_z);
    FieldEntity_SetFacingDirection(&paired->entity, 0, current->entity.base_state_flag_bits.facing_direction,
                                   1);
    paired->entity.base.visibility_bits.unknown_07 = 0;
    paired->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(paired->entity.render_object, 1);
    current->bits.movement_mode = 0;
    paired->bits.movement_mode = 0;
    current->entity.locomotion_state = 0;
    paired->entity.locomotion_state = 0;
    func_ov000_02092e24(party, current);
    func_ov000_02092e24(party, paired);
    func_ov000_0209ce18(party->paired);
    party->paired->state_bits.movement_mode = 15;
    party->paired->unknown_08c->unknown_060 = (u8)party->paired->state_bits.movement_mode;
    party->paired->unknown_090->unknown_060 = (u8)party->paired->state_bits.movement_mode;
    party->paired->unknown_088->unknown_060 = (u8)party->paired->state_bits.movement_mode;
}
