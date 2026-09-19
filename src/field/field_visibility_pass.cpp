/* Refresh culling only during ordinary field updates, excluding marker
 * entities. */
#include <game/field_area.h>
#include <game/field_entity.h>
#include <game/field_presentation.h>
#include <game/field_system.h>
extern "C" void FieldArea_UpdateEntityVisibility(FieldAreaContext *area)
{
    if ((!area->system || !area->system->scene_transition.phase) && area->unknown_23f0 == 71) {
        for (int i = 0; i < area->entity_count; ++i) {
            FieldRuntimeEntity *entity = area->entities[i];
            if (entity->base.property_00a_bits.subtype != 9)
                FieldEntity_UpdateScreenVisibility(entity);
        }
    }
}
