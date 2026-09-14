#include <game/field_party.h>
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
