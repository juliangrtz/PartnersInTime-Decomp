/*
 * Entity interaction bounds (overlay 0, 0x020A4DF8-0x020A4F88).
 *
 * The box an entity can be interacted with through, set explicitly or refreshed
 * from its resource, and the behaviour mode that decides how it responds.
 */

#include <game/field_presentation.h>
#include <game/field_timed_renderer.h>
extern "C" {

void FieldEntity_SetInteractionBounds(FieldEntity *base, s16 minimum_x, s16 maximum_y,
                       u16 width, u16 height, u16 vertical_extent)
{
    FieldRuntimeEntity *entity = (FieldRuntimeEntity *)base;
    entity->interaction_min_x = minimum_x << 12;
    entity->interaction_max_x = entity->interaction_min_x + (width << 12);
    entity->interaction_max_y = maximum_y << 12;
    entity->interaction_min_y = entity->interaction_max_y - (height << 12);
    entity->interaction_vertical_extent = vertical_extent << 12;
}
}

extern "C" {

#include <game/field_presentation.h>
/* Bounds indices are signed, including the stored script-refreshed index. */
void FieldEntity_RefreshInteractionBounds(FieldRuntimeEntity *entity) {
    int animation;
    const FieldInteractionBounds *bounds;
    if (entity->bounds_resource) {
        animation = entity->animation_id;
        if (animation >= entity->bounds_animation_count)
            animation = 0;
        entity->bounds_index =
            *(const u8 *)((u32)entity->animation_bounds +
                          ((int)sizeof(FieldAnimationBoundsIndex) * animation + 2));
        bounds = &entity->interaction_bounds[entity->bounds_index];
        FieldEntity_SetInteractionBounds(&entity->base, bounds->minimum_x, bounds->maximum_y,
                                         bounds->width, bounds->height, bounds->vertical_extent);
    } else {
        FieldEntity_SetInteractionBounds(&entity->base, -8, 0, 16, 8, 32);
    }
}

}

extern "C" {

void FieldEntity_SetBehaviorMode(FieldEntity *base, int mode)
{
    FieldRuntimeEntity *entity = (FieldRuntimeEntity *)base;
    entity->saved_presentation_flag_bits.behavior_mode = mode;
    if (entity->saved_presentation_flag_bits.behavior_mode >= 3) {
        entity->movement_direction = entity->base_state_flag_bits.facing_direction << 13;
        if (entity->saved_presentation_flag_bits.behavior_mode == 3)
            entity->render_object->set_animation((u8)entity->animation_id, 0, 1);
    }
    FieldTimedRenderer_ResetAnimationControl((FieldAnimationRenderer *)entity->render_object);
}
}
