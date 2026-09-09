#include <game/field_party.h>
#include <game/field_bounds_records.h>
#include <game/field_auxiliary.h>

extern "C" {

int FieldPartyEntity_FindPairedBounds(FieldPartyEntity *member, const FieldPairedBoundsRuntime *record)
{
    member->previous_paired_bounds_index = member->paired_bounds_index;
    member->paired_bounds_index = -1;
    if (record && !member->entity.relative_height) {
        int index = 0;
        if (!record->flags.last) {
            fx32 z = member->entity.position_z;
            do {
                if (z != record->bounds[0].z)
                    continue;
                unsigned int facing = member->entity.base_state_flag_bits.facing_direction;
                unsigned int direction = record->flags.mode;
                if (direction != ((facing - 1) & 7) && direction != facing && direction != ((facing + 1) & 7))
                    continue;
                switch (direction) {
                case 0:
                case 4:
                    if (member->entity.position_x + member->entity.navigation_min_x <
                            record->bounds[0].min_x ||
                        member->entity.position_x + member->entity.navigation_max_x > record->bounds[0].max_x)
                        continue;
                    if ((direction != 0 ||
                         member->entity.position_y + member->entity.navigation_min_y <
                             record->bounds[0].min_y ||
                         member->entity.position_y + member->entity.navigation_min_y >
                             record->bounds[0].max_y) &&
                        (direction != 4 ||
                         member->entity.position_y + member->entity.navigation_max_y <
                             record->bounds[0].min_y ||
                         member->entity.position_y + member->entity.navigation_max_y >
                             record->bounds[0].max_y))
                        continue;
                    member->paired_bounds_index = index;
                    return member->paired_bounds_index;
                case 2:
                case 6:
                    if (member->entity.position_y + member->entity.navigation_min_y <
                            record->bounds[0].min_y ||
                        member->entity.position_y + member->entity.navigation_max_y > record->bounds[0].max_y)
                        continue;
                    if ((direction != 6 ||
                         member->entity.position_x + member->entity.navigation_min_x <
                             record->bounds[0].min_x ||
                         member->entity.position_x + member->entity.navigation_min_x >
                             record->bounds[0].max_x) &&
                        (direction != 2 ||
                         member->entity.position_x + member->entity.navigation_max_x <
                             record->bounds[0].min_x ||
                         member->entity.position_x + member->entity.navigation_max_x >
                             record->bounds[0].max_x))
                        continue;
                    member->paired_bounds_index = index;
                    return member->paired_bounds_index;
                default:
                    continue;
                }
            } while (++record, ++index, !record->flags.last);
        }
    }
    return member->paired_bounds_index;
}

void FieldPartyEntity_SaveRenderSnapshot(FieldPartyEntity *member, FieldRenderSnapshot *snapshot)
{
    FieldEntity_SaveRenderSnapshot(&member->entity, snapshot);
    if (!snapshot)
        for (int i = 0; i < 6; i++)
            if (member->auxiliaries[i])
                member->auxiliaries[i]->entity.base.save_render_snapshot(0);
}

void FieldPartyEntity_RestoreRenderSnapshot(FieldPartyEntity *member, const FieldRenderSnapshot *snapshot)
{
    FieldEntity_RestoreRenderSnapshot(&member->entity, snapshot);
    if (!snapshot)
        for (int i = 0; i < 6; i++)
            if (member->auxiliaries[i])
                member->auxiliaries[i]->entity.base.restore_render_snapshot(0);
}

int FieldPartyEntity_HasActiveAction(FieldPartyEntity *member)
{
    for (int i = 0; i < 6; i++) {
        FieldAuxiliaryEntity *aux = member->auxiliaries[i];
        /* Unsigned wraparound tests the adjacent states 94 and 95. */
        const u16 state_bias = (u16)-94;
        if (aux && aux->entity.base.property_00a_bits.property_00a_flag_00 &&
            (u16)(aux->entity.locomotion_state + state_bias) <= 1)
            return 1;
    }
    return member->entity.base.property_00a_bits.property_00a_flag_00 &&
           !member->entity.base.visibility_bits.unknown_07 && member->entity.locomotion_state > 3;
}

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
    entity->paired_bounds_index = source->paired_bounds_index;
    entity->previous_paired_bounds_index = source->previous_paired_bounds_index;
    FieldEntity_CopySpatialState(&entity->entity, &source->entity);
    return entity;
}
}
