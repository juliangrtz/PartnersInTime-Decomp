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
