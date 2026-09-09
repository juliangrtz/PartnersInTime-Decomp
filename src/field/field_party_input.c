#include <game/field_party.h>
#include <game/field_area.h>

extern void func_ov000_0209418c(FieldPartyController *, int, int);
extern void func_ov000_02093e38(FieldPartyController *, int);

void FieldParty_ProcessInput(FieldPartyController *party, int held, int pressed, int change_actions)
{
    FieldAreaContext *area;
    if (!party->flags.unknown_00_01)
        return;
    area = (FieldAreaContext *)party->areas[party->flags.field_screen];
    if (!area || area->unknown_23f0 != 71 || party->flags.movement_state != area->room_id)
        return;
    if (area->transition.flags.pending || !area->flags.party_active) {
        held = 0;
        pressed = 0;
    }
    if (change_actions) {
        u32 old_actions = party->state_bits.unknown_03_04;
        FieldParty_ToggleAvailableActions(party, pressed);
        if (old_actions != party->state_bits.unknown_03_04)
            pressed = (u16)(pressed & ~(party->input_masks[0] | party->input_masks[1]));
    }
    func_ov000_0209418c(party, held, pressed);
    func_ov000_02093e38(party, held);
    FieldParty_CheckFollowerRejoin(party);
}
