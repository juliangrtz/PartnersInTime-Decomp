#include <game/field_variable_entity.h>
FieldVariableEntity *FieldVariableEntity_CopyState(FieldVariableEntity *entity,
                                                   const FieldVariableEntity *source)
{
    entity->state.flags.mode = source->state.flags.mode;
    entity->state.flags.enabled = source->state.flags.enabled;
    entity->state.flags.bobbing = source->state.flags.bobbing;
    entity->state.flags.unknown_05 = source->state.flags.unknown_05;
    entity->state.flags.bob_frame = source->state.flags.bob_frame;
    entity->state.flags.bounce_frame = source->state.flags.bounce_frame;
    entity->state.unknown_06 = source->state.unknown_06;
    entity->state.unknown_07 = source->state.unknown_07;
    entity->state.unknown_08 = source->state.unknown_08;
    entity->state.variable = source->state.variable;
    entity->state.motion.countdown = source->state.motion.countdown;
    entity->state.motion.remaining_frames = source->state.motion.remaining_frames;
    FieldEntity_CopySpatialState(&entity->entity, &source->entity);
    return entity;
}
