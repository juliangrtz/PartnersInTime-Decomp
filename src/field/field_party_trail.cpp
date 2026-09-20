/*
 * Party trail (overlay 0, 0x02091D34-0x020923DC).
 *
 * Sets up individual copies and repeating trails, updates their visibility,
 * and stops their renderers when the party action ends.
 */

#include <game/field_party.h>
#include <game/field_auxiliary.h>
#include <game/field_presentation.h>
extern "C" void func_ov000_020929b8(FieldPartyController *, FieldPartyEntity *, fx32, fx32, fx32, u8, int,
                                    int, s16, u8, u8, int);
extern "C" int FieldPartyTrail_HasActiveCopies(FieldPartyController *party, FieldPartyEntity *owner)
{
    FieldAuxiliaryEntity **slot = owner->auxiliaries;
    u32 i = 0;
    u32 count = party->histories[owner->entity.base.index & 1].flags.count;
    for (; i < count; ++i) {
        if (*slot && (*slot)->entity.base_state_flag_bits.animation_wait_enabled)
            return 1;
        ++slot;
    }
    return 0;
}
extern "C" void FieldPartyTrail_StartEmitter(FieldPartyController *party, FieldPartyEntity *owner,
                                             int resource, int animation_offset, int start, int count,
                                             u8 interval)
{
    party->trail_emitter.animation_offset = animation_offset;
    party->trail_emitter.flags.start = start;
    party->trail_emitter.flags.next = start;
    party->trail_emitter.flags.count = count;
    party->trail_emitter.timer = interval;
    party->trail_emitter.interval = interval;
    func_ov000_020929b8(party, owner, owner->entity.position_x, owner->entity.position_y,
                        owner->entity.position_z, (u8)(~((1 << start) - 1) & ((1 << (start + count)) - 1)),
                        resource, 2, 256, 2, 0, 96);
}
extern "C" void FieldPartyTrail_UpdateEmitter(FieldPartyController *party, FieldPartyEntity *owner,
                                              const FieldPartyTrailDirection *directions, s16 animation)
{
    if (!--party->trail_emitter.timer) {
        if (!owner->entity.field_state_flag_bits.vertical_motion_active) {
            u32 index = party->trail_emitter.flags.next;
            FieldAuxiliaryEntity *aux = owner->auxiliaries[index];
            aux->entity.position_x =
                owner->entity.position_x +
                (directions[owner->entity.base_state_flag_bits.facing_direction].x << 12);
            aux->entity.position_y =
                owner->entity.position_y +
                (directions[owner->entity.base_state_flag_bits.facing_direction].y << 12);
            aux->entity.position_z = owner->entity.position_z;
            aux->entity.relative_height = owner->entity.relative_height;
            aux->entity.render_object->overlap_priority_bytes[0] =
                owner->entity.render_object->overlap_priority_bytes[0];
            aux->entity.render_object->overlap_priority_bytes[1] =
                owner->entity.render_object->overlap_priority_bytes[1];
            aux->entity.render_object->overlap_priority_bytes[2] =
                owner->entity.render_object->overlap_priority_bytes[2];
            aux->entity.render_object->overlap_priority_bytes[3] =
                owner->entity.render_object->overlap_priority_bytes[3];
            FieldEntity_SetFacingDirection(&aux->entity, 0,
                                           owner->entity.base_state_flag_bits.facing_direction, 1);
            aux->entity.base.set_visible(1);
            FieldEntity_SetResourceAnimation(
                &aux->entity,
                animation + party->trail_emitter.animation_offset +
                    directions[owner->entity.base_state_flag_bits.facing_direction].animation,
                0, 0, 1);
            aux->entity.render_object->state_flags &= ~FIELD_RENDER_ANIMATION_SUPPRESSED;
            aux->entity.render_object->sort_key = owner->entity.render_object->sort_key;
            aux->entity.render_object->field_sort_key.vertical_order +=
                directions[owner->entity.base_state_flag_bits.facing_direction].order_offset << 13;
            ++index;
            if (party->trail_emitter.flags.count == index - party->trail_emitter.flags.start)
                index = party->trail_emitter.flags.start;
            party->trail_emitter.flags.next = index;
        }
        party->trail_emitter.timer = party->trail_emitter.interval;
    }
}

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
