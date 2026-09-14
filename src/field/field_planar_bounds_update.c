#include <game/field_presentation.h>

void FieldEntity2D_UpdateBounds(FieldRuntimeEntity *entity) {
    int animation, index;
    const FieldInteractionBounds *bounds;
    if (entity->bounds_resource) {
        animation = entity->animation_id;
        if (animation >= entity->bounds_animation_count)
            animation = 0;
        index = *(const s8 *)((u32)entity->animation_bounds +
                              ((int)sizeof(FieldAnimationBoundsIndex) * animation + 2));
        bounds = &entity->interaction_bounds[index];
        FieldEntity_SetInteractionBounds(&entity->base, bounds->minimum_x, bounds->maximum_y,
                                         bounds->width, bounds->height, bounds->vertical_extent);
        entity->bounds_index = index;
    } else {
        FieldEntity_SetInteractionBounds(&entity->base, -8, 0, 16, 8, 32);
    }
}
