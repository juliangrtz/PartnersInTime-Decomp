/*
 * Party manager input (overlay 0, 0x020A1400-0x020A146C).
 *
 * Routes the frame's input to the party manager.
 */

#include <game/field_party_manager.h>
void FieldPartyManager_ProcessInput(FieldPartyManager *manager, int active_held, int active_pressed,
                                    int inactive_held, u16 inactive_pressed) {
    FieldParty_ProcessInput(&manager->parties[manager->bits.active_party], active_held,
                            active_pressed, 1);
    FieldParty_ProcessInput(&manager->parties[manager->bits.active_party ^ 1], inactive_held,
                            inactive_pressed, 0);
}
