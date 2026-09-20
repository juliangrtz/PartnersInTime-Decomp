/* Reset the default actions only in movement mode 0 or 6.
 * Re-read the leader after selection reset, as the native call does. */
#include <game/field_party.h>
extern "C" void FieldParty_ResetDefaultActions(FieldPartyController *party)
{
    if (party->leader) {
        int mode = party->leader->bits.movement_mode;
        if (!mode || mode == 6) {
            FieldParty_ResetActionSelection(party);
            if (party->flags.special_contact_mode == 1 && party->leader->bits.movement_mode == 6) {
                party->actions[0] = 8;
                party->actions[1] = 8;
            }
        }
    }
}
