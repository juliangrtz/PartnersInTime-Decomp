/* Update the auxiliary figure's animation offsets and next direction modes.
 * Packed facing flags are cached before advancing the animation; extract their
 * three-bit direction only when indexing the transition table.
 */
#include <game/field_party.h>
#include <game/field_auxiliary.h>
extern "C" {
extern const s8 data_ov000_020c06dc[], data_ov000_020c0a38[];
extern const u8 data_ov000_020c09b8[];
}
extern "C" void FieldParty_UpdateAuxiliaryAnimationTransition(FieldPartyController *party,
                                                              FieldPartyEntity *member,
                                                              int immediate)
{
    FieldAuxiliaryEntity *aux = member->auxiliaries[0];
    int animation = aux->entity.render_object->animation_id;
    if (member->state_record->auxiliary_animation.flags.copy_animation_flags) {
        aux->entity.collision_state_flag_bits.unknown_07 = data_ov000_020c06dc[2 * animation];
        aux->entity.collision_state_flag_bits.unknown_03 =
            aux->entity.collision_state_flag_bits.unknown_07;
        aux->entity.runtime_flag_bits.sync_horizontal =
            aux->entity.collision_state_flag_bits.unknown_03;
        aux->entity.runtime_flag_bits.vertical_sync_dirty = data_ov000_020c06dc[2 * animation + 1];
        aux->entity.runtime_flag_bits.sync_vertical =
            aux->entity.runtime_flag_bits.vertical_sync_dirty;
    }
    const s8 *offset = data_ov000_020c0a38 +
                       (3 * animation + 24 * aux->entity.base_state_flag_bits.facing_direction);
    aux->offset_x = offset[0] << 12;
    aux->offset_y = offset[1] << 12;
    aux->offset_z = offset[2] << 12;
    if (immediate || (s32)member->entity.render_object->unknown_064 <=
                         member->entity.render_object->animation_speed) {
        u32 facing = aux->entity.base_state_flags;
        if (!immediate)
            ++animation;
        aux->target = &member->entity;
        const u8 *modes = data_ov000_020c09b8 + (2 * animation + 16 * ((facing << 27) >> 29));
        aux->bits.direction_mode = modes[0];
        aux->bits.unknown_04 = 0;
        aux->bits.unknown_05_06 = modes[1];
        aux->bits.unknown_07_08 = modes[1];
    }
}
