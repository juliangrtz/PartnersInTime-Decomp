/* Reset update order to placement order. The native caller requires at least
 * one entity; the first 32 area slots precede the linked-list head and tail. */
#include <game/field_area.h>
#include <game/field_entity.h>

extern "C" {
void FieldArea_ResetEntityUpdateOrder(FieldAreaContext *area)
{
    for (int i = 0; i < area->entity_count; ++i) {
        area->entities[i]->base.update_order = i;
        if (i > 0)
            area->entities[i]->base.update_previous = area->entities[i - 1];
        if (i < area->entity_count - 1)
            area->entities[i]->base.update_next = area->entities[i + 1];
    }
    area->first_entity = area->entities[0];
    area->first_entity->base.update_previous = 0;
    area->last_entity = area->entities[area->entity_count - 1];
    area->last_entity->base.update_next = 0;
}
}
