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
