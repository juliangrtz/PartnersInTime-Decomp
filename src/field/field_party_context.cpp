/* The active member selects a hint from its facing/contact state.
 * The inactive group always clears its hint when scene control permits. */
#include <game/field_party_manager.h>
#include <game/field_system.h>
extern "C" const u8 data_ov000_020c0af8[];
extern "C" void FieldPartyManager_UpdateContactHint(FieldPartyManager *manager)
{
    FieldSystem *system = (FieldSystem *)manager->owner;
    if (!system || !system->scene_transition.phase) {
        FieldPartyController *party = &manager->parties[manager->bits.active_party];
        FieldAreaContext *area = manager->areas[party->flags.field_screen];
        if (area && area->unknown_23f0 == 71) {
            FieldPartyEntity *member = party->members[party->flags.active_member];
            if (member) {
                FieldRuntimeEntity *other = (FieldRuntimeEntity *)member->entity.unknown_500;
                u8 mode = 0;
                if (other && member->entity.locomotion_state <= 1 &&
                    (member->entity.unknown_3a0_bits.contact_mask_b &
                     data_ov000_020c0af8[member->entity.base_state_flag_bits.facing_direction])) {
                    int contact = other->field_state_flag_bits.contact_mode;
                    if (contact == 2)
                        mode = 1;
                    else if (contact == 3)
                        mode = 2;
                }
                party->state_bits.unknown_11_12 = mode;
            } else {
                party->state_bits.unknown_11_12 = 0;
            }
        }
        manager->parties[manager->bits.active_party ^ 1].state_bits.unknown_11_12 = 0;
    }
}
