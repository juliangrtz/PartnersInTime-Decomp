/* Per-frame animation policy for planar field entities. Only the shared
 * prefix of FieldRuntimeEntity is accessed; planar allocations are 0x2B0 bytes. */
extern "C" {
#include <nitro/fx.h>
}
#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
extern "C" {
void func_0200940c(FieldRenderObject *, int);

void FieldEntity2D_UpdateAnimationState(FieldRuntimeEntity *entity)
{
    if (entity->base_state_flag_bits.animation_wait_enabled && entity->render_object &&
        entity->render_object->state_flag_bits.animation_active) {
        int mode = 0;
        switch (entity->saved_presentation_flag_bits.behavior_mode) {
        case 0:
            if (((FieldAnimationRenderer *)entity->render_object)->control.loops_remaining <= 0)
                mode = 6;
            break;
        case 4:
            if (entity->base_state_flag_bits.facing_direction !=
                entity->base_state_flag_bits.previous_facing_direction)
                entity->saved_presentation_flag_bits.unknown_14 = 1;
            mode = 2;
            /* Mode 4 shares the finished-animation reset with mode 1. */
        case 1:
            if (((FieldAnimationRenderer *)entity->render_object)->control.finished &&
                entity->locomotion_state)
                FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
            break;
        case 2:
        case 3:
        case 5:
            if (entity->locomotion_state != entity->previous_locomotion_state ||
                entity->base_state_flag_bits.facing_direction !=
                    entity->base_state_flag_bits.previous_facing_direction)
                entity->saved_presentation_flag_bits.unknown_14 = 1;
            if (entity->locomotion_state == 1) {
                if (!entity->previous_locomotion_state)
                    mode = 1;
                else
                    mode = 2;
            } else if (!entity->locomotion_state || entity->saved_presentation_flag_bits.behavior_mode == 5) {
                if (((FieldAnimationRenderer *)entity->render_object)->control.loops_remaining <= 0)
                    mode = 3;
            }
            if (mode != 3 && ((FieldAnimationRenderer *)entity->render_object)->control.finished)
                FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
            break;
        }
        int speed = entity->animation_speed;
        /* Scale walking playback by actual planar movement when requested. */
        if (entity->saved_presentation_flag_bits.unknown_15 &&
            entity->saved_presentation_flag_bits.behavior_mode - 2U <= 1) {
            fx32 movement_speed = entity->movement_speed;
            if (!movement_speed)
                movement_speed =
                    FX_Sqrt((fx32)(((s64)entity->frame_delta_x * entity->frame_delta_x + 2048) >> 12) +
                            (fx32)(((s64)entity->frame_delta_y * entity->frame_delta_y + 2048) >> 12));
            speed = (s16)(speed * movement_speed / entity->locomotion.maximum_speed);
        }
        func_0200940c(entity->render_object, speed);
        entity->base.update_animation(mode, 1);
    }
}

}
