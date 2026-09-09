#include <game/field_area.h>
extern "C" {
typedef struct SavedNav {
    u8 unknown_00[32];
    u16 enabled[2];
} SavedNav;
typedef char SavedNav_SizeCheck[sizeof(SavedNav) == 36 ? 1 : -1];
extern SavedNav data_0205a01c;
extern u16 data_0205a03c[2];
extern void func_ov000_0207ad4c(FieldAreaContext *, const FieldNavigationAdjustment *);

void FieldArea_SaveNavigationChanges(FieldAreaContext *area)
{
    int i;
    data_0205a03c[area->flags.screen] = 0;
    for (i = 0; i < area->navigation_changes->count; i++) {
        if (area->navigation_changes->entries[i].data->bits.mode >= 2 &&
            area->navigation_changes->entries[i].state == 1)
            data_0205a01c.enabled[area->flags.screen] |= 1 << i;
    }
}

static inline void RestoreChange(FieldNavigationChangeManager *manager, int index)
{
    manager->restore_change((u16)index);
}

void FieldArea_RestoreNavigationChanges(FieldAreaContext *area)
{
    int i;
    for (i = 0; i < area->navigation_changes->count; i++) {
        if (data_0205a01c.enabled[area->flags.screen] & (1 << i))
            RestoreChange(area->navigation_changes, i);
    }
}

void FieldArea_ApplyNavigationChange(FieldAreaContext *area, int index)
{
    const FieldNavigationAdjustment *records = area->navigation_changes->get_adjustments((u16)index);
    if (records) {
        int i;
        func_ov000_0207ad4c(area, records);
        for (i = 0; i < area->entity_count; i++) {
            FieldRuntimeEntity *entity = area->entities[i];
            if (entity->base.property_00a_bits.subtype != 8 && entity->base.property_00a_bits.subtype != 9 &&
                entity->base.property_00a_bits.property_00a_flag_00) {
                if (entity->runtime_flag_bits.sync_horizontal)
                    entity->runtime_flag_bits.horizontal_sync_dirty = 1;
                if (entity->runtime_flag_bits.sync_vertical)
                    entity->runtime_flag_bits.vertical_sync_dirty = 1;
            }
        }
    }
}
}
