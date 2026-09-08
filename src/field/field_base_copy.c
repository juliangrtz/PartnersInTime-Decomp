#include <game/field_entity.h>
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
FieldEntity *FieldEntity_CopyState(FieldEntity *entity, const FieldEntity *source)
{
    entity->index = source->index;
    entity->unknown_005[0] = source->unknown_005[0];
    entity->unknown_005[1] = source->unknown_005[1];
    entity->unknown_008 = source->unknown_008;
    entity->property_00a_bits.property_00a_flag_00 = source->property_00a_bits.property_00a_flag_00;
    entity->property_00a_bits.property_00a_unknown_01 = source->property_00a_bits.property_00a_unknown_01;
    entity->property_00a_bits.subtype = source->property_00a_bits.subtype;
    entity->property_00a_bits.resource_set = source->property_00a_bits.resource_set;
    entity->visibility_bits.unknown_07 = source->visibility_bits.unknown_07;
    entity->visibility_bits.unknown_08 = source->visibility_bits.unknown_08;
    entity->unknown_00c = source->unknown_00c;
    entity->property_00a_bits.heap = source->property_00a_bits.heap;
    entity->property_00a_bits.property_00a_unknown_12_14 =
        source->property_00a_bits.property_00a_unknown_12_14;
    if (source->state_payload < entity->state_payload)
        func_0202cd2c(source->state_payload, entity->state_payload, sizeof(entity->state_payload));
    else
        func_0202cc58(source->state_payload, entity->state_payload, sizeof(entity->state_payload));
    entity->self = entity;
    return entity;
}
