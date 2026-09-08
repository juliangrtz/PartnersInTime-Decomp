#include <game/field_area.h>
#include <game/field_variable_entity.h>
#include <game/heap.h>

typedef struct SavedRegions {
    u8 unknown[36];
    u16 enabled[2];
} SavedRegions;
typedef char SavedRegions_SizeCheck[sizeof(SavedRegions) == 40 ? 1 : -1];
extern SavedRegions data_0205a01c;
extern u16 data_0205a040[2];
extern void func_0202cbd4(void *, int, u32);
void FieldArea_CreateVariableEntities(FieldAreaContext *area)
{
    const FieldVariablePlacement *record = area->variable_records;
    area->variable_count = 0;
    if (record) {
        for (;;) {
            switch (record->flags.mode) {
            case 0:
                break;
            case 1:
            case 2:
            case 3: {
                FieldVariableEntity *entity =
                    (FieldVariableEntity *)GameHeap_New(sizeof(FieldVariableEntity), area->heaps.main, 0, 1);
                if (entity)
                    entity = FieldVariableEntity_InitVariablePlacement(entity, area->entity_count,
                                                                       area->flags.screen, record);
                area->entities[area->entity_count] = (FieldRuntimeEntity *)entity;
                ++area->entity_count;
                ++area->variable_count;
                break;
            }
            }
            if (record->flags.last)
                break;
            ++record;
        }
    }
}
void FieldArea_InitializeQuadRegions(FieldAreaContext *area)
{
    if (area->quad_records) {
        u32 i;
        area->quad_regions = (FieldQuadRegion *)GameHeap_NewArray(
            sizeof(FieldQuadRegion) * (area->quad_region_count + 1), area->heaps.main, 0, 1);
        func_0202cbd4(area->quad_regions, 0, sizeof(FieldQuadRegion) * (area->quad_region_count + 1));
        for (i = 0; i < area->quad_region_count; i++) {
            area->quad_regions[i].x[0] = area->quad_records[i].corners[0].x << 12;
            area->quad_regions[i].x[1] = area->quad_records[i].corners[1].x << 12;
            area->quad_regions[i].x[2] = area->quad_records[i].corners[2].x << 12;
            area->quad_regions[i].x[3] = area->quad_records[i].corners[3].x << 12;
            area->quad_regions[i].y[0] = area->quad_records[i].corners[0].y << 12;
            area->quad_regions[i].y[1] = area->quad_records[i].corners[1].y << 12;
            area->quad_regions[i].y[2] = area->quad_records[i].corners[2].y << 12;
            area->quad_regions[i].y[3] = area->quad_records[i].corners[3].y << 12;
            area->quad_regions[i].min_z = area->quad_records[i].z << 12;
            if (area->quad_records[i].height == -1)
                area->quad_regions[i].max_z = 0x7FFFFFFF;
            else
                area->quad_regions[i].max_z = (area->quad_records[i].z + area->quad_records[i].height) << 12;
            area->quad_regions[i].flags.active = (data_0205a01c.enabled[area->flags.screen] >> i) & 1;
            area->quad_regions[i].flags.last = 0;
            area->quad_regions[i].flags.mode_a = area->quad_records[i].flags.mode_a;
            area->quad_regions[i].flags.mode_b = area->quad_records[i].flags.mode_b;
            area->quad_regions[i].flags.unknown_06 = area->quad_records[i].flags.unknown_04;
            area->quad_regions[i].flags.unknown_07 = area->quad_records[i].flags.unknown_05;
            area->quad_regions[i].flags.value = area->quad_records[i].flags.value;
            area->quad_regions[i].flags.index = area->quad_records[i].target.index;
        }
        area->quad_regions[area->quad_region_count].flags.last = 1;
    } else
        area->quad_regions = 0;
    data_0205a040[area->flags.screen] = 0;
}
