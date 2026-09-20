/* Single party copies: setup, delayed visibility and cleanup.
 * Overlay 0, 0x02091D34-0x02092088.
 */

#include <game/field_party.h>
#include <game/field_auxiliary.h>
#include <game/field_presentation.h>
extern "C" void func_ov000_020929b8(FieldPartyController *, FieldPartyEntity *, fx32, fx32, fx32,
                                    u8, int, int, s16, u8, u8, int);
extern "C" void FieldPartyTrail_StartSingleCopy(FieldPartyController *party,
                                                FieldPartyEntity *owner, fx32 x, fx32 y, fx32 z,
                                                int resource, u8 relative, int animation_offset,
                                                int index, int delay, int target,
                                                const FieldPartyTrailDirection *directions)
{
    // The caller must supply a usable slot or leave at least one invisible copy.
    if (index == -1) {
        for (int i = 0; i < 6; ++i) {
            if (owner->auxiliaries[i] &&
                !owner->auxiliaries[i]->entity.base_state_flag_bits.animation_wait_enabled) {
                index = i;
                break;
            }
        }
    }
    party->trail_emitter.flags.unknown_00 = relative;
    party->trail_emitter.animation_offset = animation_offset;
    party->trail_emitter.flags.start = index;
    party->trail_emitter.flags.next = index;
    party->trail_emitter.flags.count = 1;
    party->trail_emitter.timer = 0;
    // Visibility uses the full delay argument; the countdown stores its low byte.
    party->trail_emitter.interval = delay;
    party->trail_emitter.directions = directions;
    if (relative)
        directions += owner->entity.base_state_flag_bits.facing_direction;
    x += directions->x << 12;
    y += directions->y << 12;
    func_ov000_020929b8(party, owner, x, y, z, (u8)(1 << index), resource, 2, 256, 2, delay == 0,
                        96);
    FieldEntity_SetFacingDirection(&owner->auxiliaries[index]->entity, 0,
                                   owner->entity.base_state_flag_bits.facing_direction, 1);
    FieldEntity_SetResourceAnimation(&owner->auxiliaries[index]->entity,
                                     animation_offset + directions->animation, 0, 0, 1);
    if (target == -1)
        owner->auxiliaries[index]->target = &owner->entity;
    else
        owner->auxiliaries[index]->target = &owner->auxiliaries[target]->entity;
    if (relative)
        owner->auxiliaries[index]->bits.direction_mode = 1;
    else
        owner->auxiliaries[index]->bits.direction_mode = 2;
    owner->auxiliaries[index]->bits.unknown_04 = 1;
    owner->auxiliaries[index]->bits.unknown_05_06 = 0;
    owner->auxiliaries[index]->bits.unknown_07_08 = 0;
}

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
