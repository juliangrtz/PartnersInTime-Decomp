/* Select an entered/left region, wait for incompatible party actions, then
 * start its script in the primary or enabled secondary region slot. */
#include <game/field_script_manager.h>
#include <game/field_party_manager.h>
#include <game/field_script_context.h>
extern "C" {
int func_ov000_020b7b68(FieldPartyEntity *, const FieldQuadRegion *);
int FieldScriptManager_TryRegionScript(FieldScriptManager *manager, FieldPartyEntity *member, FieldAreaContext *area)
{
    int selected = -1;
    FieldQuadRegion *regions = area->quad_regions;
    if (manager->pending_region != -1) {
        unsigned subtype = member->entity.base.property_00a_bits.subtype;
        if ((subtype == 0 && manager->region_selection.party_side == 0) ||
            (subtype == 1 && manager->region_selection.party_side == 1)) {
            if (!FieldPartyManager_HasActiveActions(area->party, manager->region_selection.party_side, subtype)) {
                selected = manager->pending_region;
                manager->pending_region = -1;
            }
        }
    } else {
        selected = func_ov000_020b7b68(member, regions);
        /* Mode 2 selects the region just left; other modes use the current hit. */
        if (selected == -1) {
            if (member->unknown_597 != -1 && regions[member->unknown_597].flags.mode_b == 2 &&
                member->unknown_597 != member->unknown_596)
                selected = member->unknown_597;
        } else if (regions[selected].flags.mode_b == 2) {
            selected = -1;
        }
        if (selected != -1 && !regions[selected].flags.unknown_07 && member->unknown_597 == member->unknown_596)
            selected = -1;
        if (selected != -1) {
            FieldQuadRegion *region = &regions[selected];
            unsigned modes = region->flags.value;
            if (modes && ((1 << member->bits.movement_mode) & modes)) {
                manager->pending_region = selected;
                manager->region_selection.party_side = (u32)member->entity.base.index >> 1;
                FieldPartyManager_CancelActions(area->party, manager->region_selection.party_side, (u16)region->flags.value);
                selected = -1;
            }
        }
    }
    if (manager->pending_region != -1 && regions[manager->pending_region].flags.active)
        manager->pending_region = -1;
    if (selected != -1 && regions[selected].flags.active)
        selected = -1;
    if (selected != -1) {
        const u16 *script = FieldScript_Lookup((FieldScriptContext *)area,
            member->entity.base.property_00a_bits.resource_set, regions[selected].flags.index);
        if (FieldScript_Begin(&manager->states[1], 0, 2, script)) {
            manager->states[1].owner_data[3] = member->entity.base.index;
            manager->states[1].flag_bits.owner_subtype = member->entity.locomotion_category;
            return 1;
        }
        if (manager->states[2].flag_bits.enabled && FieldScript_Begin(&manager->states[2], 0, 2, script)) {
            manager->states[2].owner_data[3] = member->entity.base.index;
            manager->states[2].flag_bits.owner_subtype = member->entity.locomotion_category;
            return 2;
        }
    }
    return -1;
}
}
