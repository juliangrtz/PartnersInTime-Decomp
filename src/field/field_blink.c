#include <game/field_blink.h>
extern const s8 data_ov000_020c0b10[];
extern void func_ov000_020a4608(FieldEntity *);
extern void func_ov000_020a45f4(FieldEntity *);

void FieldEntity_CopySortKeyToAnchor(FieldRuntimeEntity *entity)
{
    if (entity->render_object)
        entity->render_object->render_anchor_z = entity->render_object->sort_key;
}

void FieldBlink_Start(FieldRuntimeEntity *entity, int mode, const s8 *durations, u8 length,
                       FieldEntityVisibilityCallback first_phase, FieldEntityVisibilityCallback second_phase)
{
    if (mode) {
        if (!durations) { durations = data_ov000_020c0b10; length = 28; }
        if (!first_phase) first_phase = func_ov000_020a4608;
        entity->base_state_flag_bits.blink_mode = mode;
        if (!second_phase) second_phase = func_ov000_020a45f4;
        entity->base_state_flag_bits.blink_phase = 0;
        entity->base_state_flag_bits.blink_offset = mode == 3 ? 2 : length - 4;
        entity->blink_durations = durations;
        entity->base_state_flag_bits.blink_frames = durations[entity->base_state_flag_bits.blink_offset];
        entity->blink_first_phase = first_phase;
        entity->blink_second_phase = second_phase;
        second_phase(&entity->base);
    }
}

void FieldBlink_Update(FieldRuntimeEntity *entity)
{
    if (entity->base_state_flag_bits.blink_mode && !entity->base_state_flag_bits.blink_paused) {
        entity->base_state_flag_bits.blink_frames = entity->base_state_flag_bits.blink_frames - 1;
        if (!entity->base_state_flag_bits.blink_frames) {
            if (entity->base_state_flag_bits.blink_phase) {
                entity->blink_second_phase(&entity->base);
                entity->base_state_flag_bits.blink_offset = entity->base_state_flag_bits.blink_offset +
                    (entity->base_state_flag_bits.blink_mode == 3 ? 2 : -2);
                entity->base_state_flag_bits.blink_frames = entity->blink_durations[entity->base_state_flag_bits.blink_offset];
            } else {
                entity->blink_first_phase(&entity->base);
                entity->base_state_flag_bits.blink_frames = (entity->blink_durations + entity->base_state_flag_bits.blink_offset)[1];
            }
            entity->base_state_flag_bits.blink_phase ^= 1;
        }
        if (entity->base_state_flag_bits.blink_frames == -1) {
            if (entity->base_state_flag_bits.blink_mode == 3)
                entity->blink_second_phase(&entity->base);
            else
                entity->blink_first_phase(&entity->base);
            if (entity->base_state_flag_bits.blink_mode == 2)
                entity->base.property_00a_bits.property_00a_flag_00 = 1;
            entity->base_state_flag_bits.blink_mode = 0;
        }
    }
}

void FieldBlink_Stop(FieldRuntimeEntity *entity, int final_mode)
{
    if (final_mode != -2) {
        if (final_mode == -1) final_mode = entity->base_state_flag_bits.blink_mode;
        if (final_mode == 3) entity->blink_second_phase(&entity->base);
        else entity->blink_first_phase(&entity->base);
        if (final_mode == 2) entity->base.property_00a_bits.property_00a_flag_00 = 1;
    }
    entity->base_state_flag_bits.blink_frames = -1;
    entity->base_state_flag_bits.blink_mode = 0;
}
