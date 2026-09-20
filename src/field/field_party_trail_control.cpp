/* Party trail visibility and cleanup (overlay 0, 0x02091D34-0x02091E58). */

#include <game/field_party.h>
#include <game/field_auxiliary.h>
#include <game/field_presentation.h>

extern "C" void FieldPartyTrail_UpdateDelayedVisibility(FieldPartyController *party,
                                                        FieldPartyEntity *owner)
{
    FieldAuxiliaryEntity *aux = owner->auxiliaries[party->trail_emitter.flags.next];
    // Single-copy effects reuse the emitter interval as a one-shot countdown.
    if (party->trail_emitter.interval) {
        if (!--party->trail_emitter.interval) {
            aux->entity.base.set_visible(1);
            FieldEntity_SetFacingDirection(&aux->entity, 0,
                                           owner->entity.base_state_flag_bits.facing_direction, 1);
        }
    }
}

extern "C" void FieldPartyTrail_StopCopies(FieldPartyController *party, FieldPartyEntity *owner)
{
    // Re-read the range after each callback: it belongs to the live emitter.
    for (u32 i = party->trail_emitter.flags.start;
         i < party->trail_emitter.flags.start + party->trail_emitter.flags.count; ++i) {
        FieldAuxiliaryEntity *aux = owner->auxiliaries[i];
        if (aux->entity.saved_presentation_flag_bits.unknown_06) {
            aux->entity.base.set_visible(0);
            aux->entity.base.unknown_30();
        }
    }
}
