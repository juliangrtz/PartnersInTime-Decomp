#include <game/field_party.h>
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
