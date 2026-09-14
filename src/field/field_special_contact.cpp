#include <game/field_auxiliary.h>
/* Interaction-bearing entities extend this common prefix. Auxiliary objects
 * use their own complete 1360-byte record and owner link. */
struct ContactEntityView {
    FieldRuntimeEntity entity;
    union {
        FieldInteractionFlags interaction;
        struct {
            u32 unknown0 : 6, contact_state : 16, unknown22 : 10;
        } state;
    };
};
extern "C" int FieldSystem_CheckSpecialContact(void *unused, ContactEntityView *subject,
                                               FieldRuntimeEntity *other) {
    if (other->base.property_00a_bits.subtype == 10) {
        FieldRuntimeEntity *owner = ((FieldAuxiliaryEntity *)other)->owner;
        if (owner->locomotion_state == 24) {
            if (subject->interaction.remove_after_special_contact) {
                subject->entity.base.stop_script();
                subject->entity.base.set_visible(0);
                subject->entity.base.property_00a_bits.property_00a_flag_00 = 0;
                if (owner->unknown_500 == subject)
                    owner->unknown_500 = 0;
                return 0;
            }
        } else
            return 0;
    } else if (subject->state.contact_state)
        return 0;
    return 1;
}
