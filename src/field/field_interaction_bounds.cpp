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
