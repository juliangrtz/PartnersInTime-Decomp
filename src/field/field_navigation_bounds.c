/* Restore navigation bounds from the current animation (overlay 0, 0x020AE520). */

#include <game/field_presentation.h>
void FieldEntity_LoadNavigationBounds(FieldRuntimeEntity *entity)
{
    if (entity->bounds_resource) {
        int animation = entity->animation_id;
        const FieldInteractionBounds *bounds;
        int x, y, width, height, depth;
        if (animation >= entity->bounds_animation_count)
            animation = 0;
        entity->navigation_bounds_index =
            *(const u8 *)((u32)entity->animation_bounds +
                          ((int)sizeof(FieldAnimationBoundsIndex) * animation + 1));
        bounds = entity->navigation_bounds + entity->navigation_bounds_index;
        x = bounds->minimum_x;
        y = bounds->maximum_y;
        depth = bounds->vertical_extent;
        height = bounds->height;
        width = bounds->width;
        entity->navigation_min_x = x << 12;
        entity->navigation_max_x = entity->navigation_min_x + (width << 12);
        entity->navigation_max_y = y << 12;
        entity->navigation_min_y = entity->navigation_max_y - (height << 12);
        entity->navigation_vertical_extent = depth << 12;
    } else {
        entity->navigation_min_x = -32768;
        entity->navigation_max_x = entity->navigation_min_x + 65536;
        entity->navigation_max_y = 0;
        entity->navigation_min_y = entity->navigation_max_y - 32768;
        entity->navigation_vertical_extent = 131072;
    }
}
