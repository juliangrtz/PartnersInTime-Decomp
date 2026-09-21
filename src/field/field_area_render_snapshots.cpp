/* Preserve render state before leaving the field. Subtype 9 has no snapshot;
 * party virtual methods also capture their auxiliary entities. */
#include <game/field_area.h>

extern "C" {
void FieldArea_SaveEntityRenderSnapshots(FieldAreaContext *field)
{
    for (int i = 0; i < field->entity_count; ++i) {
        FieldRuntimeEntity *entity = field->entities[i];
        if (entity->base.property_00a_bits.subtype != 9)
            entity->base.save_render_snapshot(0);
    }
}
}
