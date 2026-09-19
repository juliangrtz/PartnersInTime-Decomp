/* Restore placement order in the render list. Like the native caller, require at least one entity. */
#include <game/field_entity.h>
#include <game/field_area.h>
extern "C" void FieldArea_ResetEntityRenderOrder(FieldAreaContext *area)
{
    for (int i = 0; i < area->entity_count; ++i) {
        area->entities[i]->base.render_order = i;
        if (i > 0)
            area->entities[i]->base.render_previous = area->entities[i - 1];
        if (i < area->entity_count - 1)
            area->entities[i]->base.render_next = area->entities[i + 1];
    }
    area->first_render_entity = area->entities[0];
    area->first_render_entity->base.render_previous = 0;
    area->last_render_entity = area->entities[area->entity_count - 1];
    area->last_render_entity->base.render_next = 0;
}
