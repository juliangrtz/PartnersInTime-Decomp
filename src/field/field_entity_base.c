#include <game/field_entity.h>

extern FieldEntityVTable data_ov000_020c0fc4[];
extern void GameHeap_Delete(FieldEntity *entity);
extern void func_0202cbd4(void *destination, int value, u32 size);

/* Metrowerks emits C functions in reverse source order. */
FieldEntity *FieldEntity_InitializeB(FieldEntity *entity) {
    entity->vtable = data_ov000_020c0fc4;
    func_0202cbd4((u8 *)entity + sizeof(entity->vtable), 0,
                  sizeof(*entity) - sizeof(entity->vtable));
    return entity;
}

FieldEntity *FieldEntity_InitializeA(FieldEntity *entity) {
    entity->vtable = data_ov000_020c0fc4;
    func_0202cbd4((u8 *)entity + sizeof(entity->vtable), 0,
                  sizeof(*entity) - sizeof(entity->vtable));
    return entity;
}

FieldEntity *FieldEntity_SetVTableB(FieldEntity *entity) {
    entity->vtable = data_ov000_020c0fc4;
    return entity;
}

FieldEntity *FieldEntity_Finalize(FieldEntity *entity) {
    entity->vtable = data_ov000_020c0fc4;
    GameHeap_Delete(entity);
    return entity;
}

FieldEntity *FieldEntity_SetVTableA(FieldEntity *entity) {
    entity->vtable = data_ov000_020c0fc4;
    return entity;
}

void FieldEntity_NoOp(void) {
}

void FieldEntity_ResetActionState(FieldEntity *entity) {
    entity->state_flags &= ~FIELD_ENTITY_STATE_ACTIVE;
    entity->action_timer = 0;
}

void FieldEntity_SetFlag02(FieldEntity *entity) {
    entity->state_flags |= FIELD_ENTITY_STATE_FLAG_02;
}

void FieldEntity_ClearFlag02(FieldEntity *entity) {
    entity->state_flags &= ~FIELD_ENTITY_STATE_FLAG_02;
}

int FieldEntity_GetInteractionState(FieldEntity *entity, int property) {
    switch (property) {
    case 4:
        return entity->property_00a_bits.property_00a_flag_00;
    case 5:
        if (!entity->state_flag_bits.active) {
            return 0;
        }
        if (entity->state_flag_bits.flag_02) {
            return 3;
        }
        if (entity->state_flag_bits.interaction_state == 3) {
            return 2;
        }
        return 1;
    default:
        return 0;
    }
}

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
