#include <game/field_party.h>
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
FieldPartyEntity *FieldPartyEntity_CopyState(FieldPartyEntity *entity, const FieldPartyEntity *source)
{
    entity->bytes.saved_collision = source->bytes.saved_collision;
    entity->bytes.collision_saved = source->bytes.collision_saved;
    entity->bits.saved_collision_faces = source->bits.saved_collision_faces;
    entity->bits.collision_faces_saved = source->bits.collision_faces_saved;
    entity->bits.movement_mode = source->bits.movement_mode;
    entity->bits.unknown_22 = source->bits.unknown_22;
    entity->bits.unknown_23 = source->bits.unknown_23;
    entity->bits.unknown_24 = source->bits.unknown_24;
    entity->bits.unknown_25_30 = source->bits.unknown_25_30;
    if (&source->movement < &entity->movement)
        func_0202cd2c(&source->movement, &entity->movement, sizeof(entity->movement));
    else
        func_0202cc58(&source->movement, &entity->movement, sizeof(entity->movement));
    entity->saved_animation_speed = source->saved_animation_speed;
    entity->presentation.unknown_00 = source->presentation.unknown_00;
    entity->presentation.saved_behavior = source->presentation.saved_behavior;
    entity->presentation.behavior_saved = source->presentation.behavior_saved;
    entity->presentation.unknown_05 = source->presentation.unknown_05;
    entity->presentation.resource_index = source->presentation.resource_index;
    entity->presentation.unknown_14 = source->presentation.unknown_14;
    entity->presentation.unknown_15 = source->presentation.unknown_15;
    entity->unknown_594.unknown_00 = source->unknown_594.unknown_00;
    entity->unknown_594.unknown_01_07 = source->unknown_594.unknown_01_07;
    entity->unknown_595 = source->unknown_595;
    entity->unknown_596 = source->unknown_596;
    entity->unknown_597 = source->unknown_597;
    entity->unknown_598 = source->unknown_598;
    entity->unknown_599 = source->unknown_599;
    FieldEntity_CopySpatialState(&entity->entity, &source->entity);
    return entity;
}
