/* Construct the room's placed entities, retaining its two resource sets. */
extern "C" {
#include <game/heap.h>
}
#include <game/field_area.h>
#include <game/field_deferred_entity.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_party.h>
#include <game/field_resources.h>
#include <game/field_room_transition.h>
#include <game/field_variable_entity.h>
extern "C" {
void func_0202cbd4(void *, int, u32);

FieldEntity *func_ov000_020a2d04(FieldEntity *, int, const FieldSpawnRecord *, int, int, u8);

void FieldArea_CreatePlacedEntities(FieldAreaContext *area)
{
    FieldResourceContext *resources = (FieldResourceContext *)area;
    u32 enabled_mask = area->unknown_24c0;
    if (data_02048f18[area->room_id].type == 2)
        enabled_mask |= 15;
    func_0202cbd4(area->entities, 0, 32 * sizeof(FieldRuntimeEntity *));
    area->entity_count = 0;
    for (int set = 0; set < 2; ++set) {
        for (int i = 0; i < resources->spawn_counts[set]; ++i) {
            bool enabled = ((enabled_mask >> area->entity_count) & 1) != 0;
            if (resources->spawn_records[set][i].unknown_14 != 0xFFFF)
                enabled =
                    (enabled &
                     (VM_ReadVariable(resources->spawn_records[set][i].unknown_14, 0, 0) ^ 1)) != 0;
            switch (resources->spawn_records[set][i].flags.bits.subtype) {
            case 0: {
                FieldPartyEntity *entity = (FieldPartyEntity *)GameHeap_New(
                    sizeof(FieldPartyEntity), area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldPartyEntity_InitPlacement(entity, area->entity_count,
                                                            &resources->spawn_records[set][i],
                                                            area->flags.screen, set, enabled, 0);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 1: {
                FieldPartyEntity *entity = (FieldPartyEntity *)GameHeap_New(
                    sizeof(FieldPartyEntity), area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldPartyEntity_InitPlacement(entity, area->entity_count,
                                                            &resources->spawn_records[set][i],
                                                            area->flags.screen, set, enabled, 1);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 2: {
                FieldRuntimeEntity *entity = (FieldRuntimeEntity *)GameHeap_New(
                    FIELD_ENTITY_3D_SIZE, area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldEntity3D_InitPlacementBase(entity, area->entity_count,
                                                             &resources->spawn_records[set][i],
                                                             area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 3: {
                FieldRuntimeEntity *entity = (FieldRuntimeEntity *)GameHeap_New(
                    FIELD_ENTITY_3D_SIZE, area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldEntity3D_InitPlacementBase(entity, area->entity_count,
                                                             &resources->spawn_records[set][i],
                                                             area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 4: {
                FieldDeferredEntity *entity = (FieldDeferredEntity *)GameHeap_New(
                    sizeof(FieldDeferredEntity), area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldDeferredEntity_Init(entity, area->entity_count,
                                                      &resources->spawn_records[set][i],
                                                      area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 5: {
                FieldRuntimeEntity *entity = (FieldRuntimeEntity *)GameHeap_New(
                    FIELD_ENTITY_3D_SIZE, area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldEntity3D_InitPlacementBase(entity, area->entity_count,
                                                             &resources->spawn_records[set][i],
                                                             area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 6: {
                FieldRuntimeEntity *entity = (FieldRuntimeEntity *)GameHeap_New(
                    FIELD_ENTITY_3D_SIZE, area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldEntity3D_InitPlacementBase(entity, area->entity_count,
                                                             &resources->spawn_records[set][i],
                                                             area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 7: {
                FieldVariableEntity *entity = (FieldVariableEntity *)GameHeap_New(
                    sizeof(FieldVariableEntity), area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldVariableEntity_InitPlacement(entity, area->entity_count,
                                                               &resources->spawn_records[set][i],
                                                               area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 8: {
                FieldRuntimeEntity *entity = (FieldRuntimeEntity *)GameHeap_New(
                    FIELD_ENTITY_2D_SIZE, area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldEntity2D_InitPlacementBase(entity, area->entity_count,
                                                             &resources->spawn_records[set][i],
                                                             area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            case 9: {
                FieldEntity *entity =
                    (FieldEntity *)GameHeap_New(sizeof(FieldEntity), area->heaps.main, 0, 1);
                if (entity)
                    entity = func_ov000_020a2d04(entity, area->entity_count,
                                                 &resources->spawn_records[set][i],
                                                 area->flags.screen, set, enabled);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                break;
            }
            }
            ++area->entity_count;
        }
    }
}
}
