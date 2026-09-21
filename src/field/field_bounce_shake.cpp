/*
 * Field entity bounce and horizontal shake (overlay 0, 0x020B451C-0x020B4728).
 * Pauses the entity script while it rises and lands, waits three updates,
 * then shakes for sixty updates before resuming the script.
 */
#include <game/field_entity_motion.h>
extern "C" {
extern const s8 data_ov000_020c0b30[4];

void FieldEntity3D_StartBounceShake(FieldRuntimeEntity *entity)
{
    entity->bounce_shake.phase = 1;
    entity->base.pause_script();
    FieldVertical_Start(entity, 16137, 512, -1);
    entity->unknown_3a0_bits.unknown_26_31 = 0;
}

void FieldEntity3D_StopBounceShake(FieldRuntimeEntity *entity)
{
    if (entity->bounce_shake.phase) {
        entity->screen_offset_x = 0;
        entity->bounce_shake.phase = 0;
        entity->base.resume_script();
    }
}

void FieldEntity3D_UpdateBounceShake(FieldRuntimeEntity *entity)
{
    switch (entity->bounce_shake.phase) {
    case 0: /* Inactive. */
        break;
    case 1: /* Wait for the vertical motion to finish. */
        if (entity->vertical_velocity <= 0)
            entity->unknown_3a0_bits.unknown_26_31 = 63;
        if (!entity->field_state_flag_bits.vertical_motion_active) {
            entity->bounce_shake.remaining = 3;
            entity->bounce_shake.phase = 2;
        }
        break;
    case 2: /* Brief landing pause. */
        --entity->bounce_shake.remaining;
        if (!entity->bounce_shake.remaining) {
            entity->bounce_shake.remaining = 60;
            entity->bounce_shake.phase = 3;
        }
        break;
    case 3: /* Horizontal offsets are signed bytes from the native table. */
        --entity->bounce_shake.remaining;
        if (!entity->bounce_shake.remaining)
            FieldEntity3D_StopBounceShake(entity);
        else
            entity->screen_offset_x = data_ov000_020c0b30[entity->bounce_shake.remaining & 3];
        break;
    }
}

}
