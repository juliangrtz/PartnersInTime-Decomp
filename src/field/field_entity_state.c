#include <game/field_entity.h>

extern void *data_ov000_020c0fc4[];
extern void func_02029bb0(FieldEntity *entity);
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
    func_02029bb0(entity);
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
