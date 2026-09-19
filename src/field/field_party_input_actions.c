/*
 * Party input and actions (overlay 0, 0x0209CE00-0x0209D254).
 *
 * Reads the pad for the party: toggling between the available actions, setting a
 * member's action, and refreshing or resetting the selection.
 */

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

extern const s16 data_ov000_020c0724[], data_ov000_020c0726[];
void FieldParty_InitializeIndicator(FieldPartyController *party, int visible, int screen,
                                    int layout, int style, int mode) {
    party->state_bits.unknown_00 = visible;
    party->state_bits.unknown_14_17 = style;
    party->state_bits.movement_mode = mode;
    if (party->flags.unknown_00_01 == 3) {
        int offset = 8 * screen + 4 * (layout - 1);
        party->indicator_x = data_ov000_020c0724[offset + 2 * party->flags.special_contact_mode];
        party->indicator_y = data_ov000_020c0726[offset + 2 * party->flags.special_contact_mode];
    } else {
        party->indicator_x = 242;
        party->indicator_y = 15;
    }
    party->indicator_slide_y = visible ? 0 : !party->flags.field_screen ? -52 : 52;
    party->state_bits.unknown_05 = party->flags.field_screen;
}

#include <game/save_data.h>

extern const u8 *const data_ov000_020c0f4c[];
extern const u16 data_ov000_020c06bc[];

void FieldParty_ToggleAvailableActions(FieldPartyController *party, int pressed)
{
    int i;
    if (!party->state_bits.unknown_01 || !party->state_bits.unknown_02 || !party->indicator_bits.visible ||
        party->indicator_bits.visibility_transition || !(pressed & 0x300))
        return;
    for (i = 0; i < 2; i++) {
        if (party->flags.unknown_00_01 & (1 << i)) {
            u16 variable = data_ov000_020c06bc[(2 * i + 4 * party->flags.special_contact_mode) +
                                               (((party->state_bits.unknown_03_04 >> i) & 1) ^ 1)];
            if (variable == 65535 || VM_ReadVariable(variable, 0, 0))
                party->state_bits.unknown_03_04 ^= 1 << i;
        }
    }
    FieldParty_RefreshActions(party);
}
void FieldParty_SetMemberAction(FieldPartyController *party, int member, int action)
{
    if (action == -1) {
        if (party->flags.special_contact_mode == 1 && party->leader->bits.movement_mode == 6)
            action = 8;
        else {
            const u8 *choices =
                data_ov000_020c0f4c[2 * party->flags.movement_active + party->flags.special_contact_mode] +
                member;
            action = choices[2 * ((party->state_bits.unknown_03_04 >> member) & 1)];
        }
    }
    if (action == 9)
        action += gSaveData[1301];
    party->actions[member] = action;
}
void FieldParty_RefreshActions(FieldPartyController *party)
{
    FieldParty_SetMemberAction(party, 0, -1);
    FieldParty_SetMemberAction(party, 1, -1);
}
void FieldParty_ResetActionSelection(FieldPartyController *party)
{
    party->state_bits.unknown_03_04 = 0;
    FieldParty_RefreshActions(party);
}
