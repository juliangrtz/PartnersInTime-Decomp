#include <game/field_variable_entity.h>
extern FieldEntityVTable data_ov000_020c1310;
extern const FieldSpawnRecord data_ov000_020c0c80;
extern const s8 data_ov000_020c0c64[];
extern void func_0202cbd4(void *, int, u32);
/* Match the original signed 32-bit masks extended to the 64-bit policy. */
#define SetCollisionChannel(entity, shift, policy)                                                           \
    do {                                                                                                     \
        (entity)->collision_policy &= ~(s32)(7u << (shift));                                                 \
        if (policy)                                                                                          \
            (entity)->collision_policy |= (s32)((u32)(policy) << (shift));                                   \
    } while (0)
FieldVariableEntity *FieldVariableEntity_InitPlacement(FieldVariableEntity *entity, int index,
                                                       const FieldSpawnRecord *spawn, int argument,
                                                       int resource_set, u8 animation_enabled)
{
    FieldEntity3D_InitPlacement(&entity->entity, index, spawn, argument, resource_set, animation_enabled);
    entity->entity.base.vtable = &data_ov000_020c1310;
    func_0202cbd4(&entity->state, 0, sizeof(entity->state));
    entity->state.flags.mode = 0;
    entity->state.flags.enabled = 1;
    entity->state.flags.unknown_04 = 1;
    entity->state.flags.unknown_05 = 0;
    entity->state.flags.unknown_06_13 = 0;
    entity->state.flags.unknown_14_21 = -1;
    SetCollisionChannel(&entity->entity, 0, 7);
    SetCollisionChannel(&entity->entity, 3, 7);
    SetCollisionChannel(&entity->entity, 6, 7);
    SetCollisionChannel(&entity->entity, 9, 7);
    SetCollisionChannel(&entity->entity, 12, 0);
    SetCollisionChannel(&entity->entity, 15, 0);
    SetCollisionChannel(&entity->entity, 18, 0);
    SetCollisionChannel(&entity->entity, 21, 0);
    SetCollisionChannel(&entity->entity, 24, 0);
    SetCollisionChannel(&entity->entity, 27, 0);
    SetCollisionChannel(&entity->entity, 30, 7);
    entity->entity.unknown_3a0_bits.unknown_26_31 = 16;
    return entity;
}
FieldVariableEntity *FieldVariableEntity_InitVariablePlacement(FieldVariableEntity *entity, int index,
                                                               int argument,
                                                               const FieldVariablePlacement *placement)
{
    FieldEntity3D_InitPlacement(&entity->entity, index, &data_ov000_020c0c80, argument, 0, 1);
    entity->entity.base.vtable = &data_ov000_020c1310;
    func_0202cbd4(&entity->state, 0, sizeof(entity->state));
    entity->entity.position_x = placement->x << 12;
    entity->entity.position_y = placement->y << 12;
    entity->entity.position_z = placement->z << 12;
    /* Preserve the original wrapped 16-bit subtraction of variable bank 0x2000. */
    entity->entity.unknown_3d4 = placement->variable + (u16)-0x2000;
    entity->state.flags.mode = placement->flags.mode;
    entity->state.flags.enabled = VM_ReadVariable((u16)entity->entity.unknown_3d4, 0, 0) ^ 1;
    entity->state.flags.unknown_04 = 1;
    entity->state.flags.unknown_05 = 0;
    entity->state.flags.unknown_06_13 = 0;
    entity->state.flags.unknown_14_21 = -1;
    entity->state.remaining_uses = placement->flags.parameter;
    entity->state.unknown_07 = placement->motion.amount_index;
    entity->state.motion.unknown_00 = 0;
    entity->state.motion.unknown_01_15 =
        entity->state.unknown_06 * data_ov000_020c0c64[placement->motion.direction];
    entity->state.unknown_08 = placement->unknown_02;
    entity->state.variable = placement->variable;
    entity->entity.saved_presentation_flag_bits.behavior_mode = 1;
    SetCollisionChannel(&entity->entity, 0, 7);
    SetCollisionChannel(&entity->entity, 3, 7);
    SetCollisionChannel(&entity->entity, 6, 0);
    SetCollisionChannel(&entity->entity, 9, 7);
    SetCollisionChannel(&entity->entity, 12, 0);
    SetCollisionChannel(&entity->entity, 15, 0);
    SetCollisionChannel(&entity->entity, 18, 0);
    SetCollisionChannel(&entity->entity, 21, 0);
    SetCollisionChannel(&entity->entity, 24, 0);
    SetCollisionChannel(&entity->entity, 27, 0);
    SetCollisionChannel(&entity->entity, 30, 7);
    entity->entity.unknown_3a0_bits.unknown_26_31 = 16;
    return entity;
}
#undef SetCollisionChannel
