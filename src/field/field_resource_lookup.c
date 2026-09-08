#include <game/field_resources.h>
FieldPrimaryResource *FieldResources_FindShared(void *field_context, u32 id)
{
    FieldResourceContext *field = (FieldResourceContext *)field_context;
    int i;
    for (i = 0; i < 85; ++i) {
        if (field->shared_resources[i].id == id)
            return &field->shared_resources[i];
    }
    return 0;
}

int FieldResources_UsesAlternateHeap(void *field_context, int set, int resource_index)
{
    FieldResourceContext *field = (FieldResourceContext *)field_context;
    int i;
    int count = field->entity_count - field->trailing_entity_count;
    for (i = 0; i < count; ++i) {
        FieldRuntimeEntity *entity = field->entities[i];
        if (entity->base.property_00a_bits.subtype != 9 &&
            set == entity->base.property_00a_bits.resource_set && entity->resource_index == resource_index)
            return entity->saved_presentation_flag_bits.resource_set;
    }
    return 0;
}
