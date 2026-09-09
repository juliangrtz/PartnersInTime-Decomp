extern "C" {
#include <nitro/fx.h>
}
#include <game/field_entity.h>
#include <game/field_timed_renderer.h>
extern "C" {
extern const u8 data_ov000_020c0b38[];
void func_020093b4(FieldRenderObject *, int);
void func_0200940c(FieldRenderObject *, int);

void FieldEntity3D_UpdateAnimationState(FieldRuntimeEntity *entity)
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
            } else if (entity->locomotion_state == 2)
                mode = 4;
            else if (entity->locomotion_state == 3) {
                if (entity->falling_frames >= entity->unknown_370 || entity->field_state_flag_bits.unknown_07)
                    mode = 5;
                else
                    mode = 4;
            }
            if (mode != 3 && ((FieldAnimationRenderer *)entity->render_object)->control.finished)
                FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
            break;
        }
        int speed = entity->animation_speed;
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

int FieldEntity3D_ShouldShowShadow(FieldRuntimeEntity *entity)
{
    return entity->base_state_flag_bits.animation_wait_enabled && !entity->base.visibility_bits.unknown_07 &&
           entity->render_object->state_flag_bits.animation_active &&
           entity->field_state_flag_bits.shadow_enabled &&
           ((entity->relative_height > 0 && entity->support_entity_index == -1) ||
            entity->field_state_flag_bits.unknown_16);
}

void FieldEntity3D_UpdateShadow(FieldRuntimeEntity *entity)
{
    if (entity->auxiliary_render_object) {
        if (FieldEntity3D_ShouldShowShadow(entity)) {
            func_020093b4(entity->auxiliary_render_object, 1);
            int animation = data_ov000_020c0b38[entity->field_state_flag_bits.shadow_style];
            if (entity->field_state_flag_bits.shadow_style - 1U <= 1) {
                int size;
                if (entity->support_clearance >= 20 * 4096)
                    size = 0;
                else if (entity->support_clearance >= 10 * 4096)
                    size = 1;
                else
                    size = 2;
                animation += size;
            }
            if (entity->auxiliary_render_object->resource_animation != animation)
                entity->auxiliary_render_object->set_animation((u8)animation, -1, 1);
        } else
            func_020093b4(entity->auxiliary_render_object, 0);
    }
}

void FieldEntity3D_UpdateScreenPosition(FieldRuntimeEntity *entity, s16 camera_x, s16 camera_y)
{
    if (entity->base_state_flag_bits.animation_wait_enabled) {
        entity->screen_x = entity->position_x / 4096 - camera_x;
        entity->screen_y = (entity->position_y - entity->position_z) / 4096 - camera_y;
        FieldRenderObject *render = entity->render_object;
        if (render) {
            s16 screen_y = entity->screen_y, offset_y = entity->screen_offset_y;
            render->animation_offset_x = entity->screen_x + entity->screen_offset_x;
            render->animation_offset_y = screen_y + offset_y;
            if (entity->field_state_flag_bits.body_collision_enabled)
                entity->render_object->field_sort_key.vertical_order =
                    (4096 - (entity->position_y + entity->interaction_max_y) / 4096) << 8;
            entity->render_object->field_sort_key.entity_index = entity->base.index;
            if (entity->auxiliary_render_object) {
                entity->unknown_3d0 = entity->screen_y + entity->support_clearance / 4096;
                s16 auxiliary_y = entity->unknown_3d0;
                FieldRenderObject *auxiliary = entity->auxiliary_render_object;
                auxiliary->animation_offset_x = entity->screen_x;
                auxiliary->animation_offset_y = auxiliary_y;
                if (entity->support_entity) {
                    entity->auxiliary_render_object->sort_key =
                        entity->support_entity->render_object->sort_key - 64;
                    entity->auxiliary_render_object->overlap_priority_bytes[0] =
                        entity->support_entity->render_object->overlap_priority_bytes[0];
                } else {
                    entity->auxiliary_render_object->field_sort_key.layer =
                        entity->render_object->field_sort_key.layer;
                    entity->auxiliary_render_object->field_sort_key.vertical_order = -1;
                }
                entity->auxiliary_render_object->field_sort_key.entity_index = entity->base.index;
            }
        }
    }
}
}
