#include <game/field_presentation.h>
int FieldEntity2D_IsOutsideScreen(FieldRuntimeEntity *entity) {
    int x = entity->screen_x;
    int result = 1;
    int vertical = 1;
    int outside = 1;
    if (x + entity->interaction_min_x / 4096 < 256)
        if (x + entity->interaction_max_x / 4096 >= 0)
            outside = 0;
    if (!outside &&
        entity->screen_y +
                (entity->interaction_min_y - entity->interaction_vertical_extent) / 4096 <
            192)
        vertical = 0;
    if (!vertical && entity->screen_y + 4 >= 0)
        result = 0;
    return result;
}
