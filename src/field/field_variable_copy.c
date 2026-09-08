#include <game/field_variable_entity.h>
FieldVariableEntity *FieldVariableEntity_CopyState(FieldVariableEntity *entity,
                                                   const FieldVariableEntity *source)
{
    entity->state.flags.mode = source->state.flags.mode;
    entity->state.flags.enabled = source->state.flags.enabled;
    entity->state.flags.unknown_04 = source->state.flags.unknown_04;
    entity->state.flags.unknown_05 = source->state.flags.unknown_05;
    entity->state.flags.unknown_06_13 = source->state.flags.unknown_06_13;
    entity->state.flags.unknown_14_21 = source->state.flags.unknown_14_21;
    entity->state.unknown_06 = source->state.unknown_06;
    entity->state.unknown_07 = source->state.unknown_07;
    entity->state.unknown_08 = source->state.unknown_08;
    entity->state.variable = source->state.variable;
    entity->state.motion.unknown_00 = source->state.motion.unknown_00;
    entity->state.motion.unknown_01_15 = source->state.motion.unknown_01_15;
    FieldEntity_CopySpatialState(&entity->entity, &source->entity);
    return entity;
}
