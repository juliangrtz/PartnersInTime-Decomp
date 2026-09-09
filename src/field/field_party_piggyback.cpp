extern "C" {
#include <nitro/fx.h>
#include <nitro/fx_atan.h>
#include <game/field_geometry.h>
}
#include "field_party_internal.h"

extern "C" {
void func_ov000_020b86ec(FieldPartyEntity *);
int func_ov000_020aa5b0(FieldPartyEntity *, fx32 *, fx32 *);
}

#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

extern "C" void FieldParty_BeginAirborneTransfer(FieldPartyController *party, int member)
{
    FieldPartyEntity *current = party->members[member];
    FieldPartyEntity *paired = party->paired->members[member];
    fx32 offset_x, offset_y, corner_x[4], corner_y[4];
    party->flags.previous_field_screen = party->flags.field_screen;
    paired->state_record->bounds_snapshot.body_vertical_extent = paired->entity.body_vertical_extent;
    paired->state_record->bounds_snapshot.navigation_vertical_extent =
        paired->entity.navigation_vertical_extent;
    FieldParty_BindActionResource(party, paired, member + 45, 256, 0);
    FieldParty_BindActionResource(party, current, member + 16, 256, 0);
    FieldGeometry_GetDirectionVector(paired->entity.base_state_flag_bits.facing_direction ^ 4, 16384,
                                     &offset_x, &offset_y);
    FieldEntity3D_SetPosition(&current->entity, paired->entity.position_x + offset_x,
                              paired->entity.position_y + offset_y, paired->entity.position_z + 95990);
    FieldEntity_SetFacingDirection(&current->entity, 0, paired->entity.base_state_flag_bits.facing_direction,
                                   1);
    func_ov000_020b86ec(paired);
    FieldPartyEntity_CopyPartnerPlanarBounds(current);
    FieldNavigationSurface *surface = current->entity.navigation_surfaces;
    if (surface) {
        fx32 limit = current->entity.navigation_min_x +
                     MIN(current->entity.position_x, current->entity.previous_position_x) - 524288 -
                     current->entity.locomotion.starting_speed;
        while (!surface->bits.end && surface->sort_x < limit)
            ++surface;
    }
    current->entity.navigation_cursor = surface;
    current->entity.navigation_scan_limit =
        current->entity.navigation_max_x +
        MAX(current->entity.position_x, current->entity.previous_position_x) + 65536;
    current->entity.swept_min_x = current->entity.navigation_min_x +
                                  MIN(current->entity.position_x, current->entity.previous_position_x) -
                                  current->entity.locomotion.starting_speed;
    current->entity.swept_min_y = current->entity.navigation_min_y +
                                  MIN(current->entity.position_y, current->entity.previous_position_y) -
                                  current->entity.locomotion.starting_speed;
    current->entity.swept_max_x = current->entity.locomotion.starting_speed +
                                  (current->entity.navigation_max_x +
                                   MAX(current->entity.position_x, current->entity.previous_position_x));
    current->entity.swept_max_y = current->entity.locomotion.starting_speed +
                                  (current->entity.navigation_max_y +
                                   MAX(current->entity.position_y, current->entity.previous_position_y));
    corner_x[0] = corner_x[1] = current->entity.position_x + current->entity.navigation_min_x;
    corner_x[2] = corner_x[3] = current->entity.position_x + current->entity.navigation_max_x;
    corner_y[0] = corner_y[3] = current->entity.position_y + current->entity.navigation_min_y;
    corner_y[1] = corner_y[2] = current->entity.position_y + current->entity.navigation_max_y;
    if (func_ov000_020aa5b0(current, corner_x, corner_y)) {
        FieldParty_FinishAirborneTransfer(party, member);
        return;
    }
    current->entity.base.visibility_bits.unknown_07 = 0;
    current->entity.saved_presentation_flag_bits.unknown_31 = 0;
    func_020093b4(current->entity.render_object, 1);
    current->entity.runtime_flag_bits.sync_horizontal = 0;
    current->entity.runtime_flag_bits.unknown_08 = 1;
    FieldEntity3D_SetPosition(&current->entity, paired->entity.position_x, paired->entity.position_y,
                              paired->entity.position_z + 81920);
    FieldVertical_Start(&current->entity, 14070, 1281, 0);
    current->entity.locomotion_state = 57;
    GameAudio_PlayEffectDelayed(218, 0, -1);
}

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
    FieldParty_BindActionResource(party, paired, member + 39, 256, 0);
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
    FieldParty_BindActionResource(party, current, member + 43, 256, 0);
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
    FieldParty_RefreshActions(party->paired);
    party->paired->state_bits.movement_mode = 15;
    party->paired->unknown_08c->unknown_060 = (u8)party->paired->state_bits.movement_mode;
    party->paired->unknown_090->unknown_060 = (u8)party->paired->state_bits.movement_mode;
    party->paired->unknown_088->unknown_060 = (u8)party->paired->state_bits.movement_mode;
}
