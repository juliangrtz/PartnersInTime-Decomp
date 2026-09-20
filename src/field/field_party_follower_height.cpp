/* Update the follower height gate only for the party's current field room.
 * Both directional separation checks run before their low result bits are
 * combined. Clearing the gate restarts the follower path; setting it also
 * checks the two-level entity link to avoid gating a linked leader.
 */
#include <game/field_party.h>
#include <game/field_area.h>
extern "C" {
u8 func_ov000_020a87a4(FieldRuntimeEntity *, FieldRuntimeEntity *, int);
void func_ov000_020b90b4(FieldPartyEntity *, void *, FieldPartyEntity *, int, fx32);
void FieldParty_UpdateFollowerHeightGate(FieldPartyController *party)
{
    FieldAreaContext *area = (FieldAreaContext *)party->areas[party->flags.field_screen];
    if (!area || area->unknown_23f0 != 71 || party->flags.movement_state != area->room_id)
        return;
    if (party->flags.unknown_00_01 == 3) {
        if (party->flags.movement_active &&
            !party->follower->entity.base.visibility_bits.unknown_07 &&
            !party->follower->entity.base.visibility_bits.unknown_08) {
            FieldPartyEntity *follower = party->follower;
            u8 separated = func_ov000_020a87a4(&party->leader->entity, &follower->entity, 0);
            party->flags.unknown_19 =
                separated | func_ov000_020a87a4(&follower->entity, &party->leader->entity, 0);
        } else
            party->flags.unknown_19 = 0;
    } else
        party->flags.unknown_19 = 0;
    if (!party->flags.movement_active || party->follower->entity.base.visibility_bits.unknown_07 ||
        party->follower->entity.base.visibility_bits.unknown_08)
        return;
    if (party->follower_height.flags.height_wait &&
        ((!party->follower->entity.field_state_flag_bits.previous_vertical_motion_active &&
          party->follower->entity.relative_height > 0 &&
          party->follower->entity.position_z <= party->follower->entity.previous_position_z) ||
         !party->flags.unknown_19)) {
        party->follower_height.flags.height_wait = 0;
        func_ov000_020b90b4(party->follower, 0, 0, party->follower_height.distance, 0);
    }
    if (!party->follower_height.flags.unknown_02 &&
        !party->follower->entity.field_state_flag_bits.vertical_motion_active &&
        party->leader->entity.position_z < party->follower->entity.position_z &&
        party->flags.unknown_19) {
        FieldRuntimeEntity *link = (FieldRuntimeEntity *)party->follower->entity.unknown_4f0;
        if (!link || !(link = (FieldRuntimeEntity *)link->unknown_4f0) ||
            link->base.index != party->leader->entity.base.index)
            party->follower_height.flags.height_wait = 1;
    }
}
}
