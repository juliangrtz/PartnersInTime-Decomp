/* Check the active leader against paired bounds only while ordinary field
 * control is available and the area is not fully faded out. */
#include <game/field_party_manager.h>
#include <game/field_system.h>
extern "C" void func_ov000_020802e0(FieldAreaContext *, int, const FieldPairedBoundsRuntime *, int);
extern "C" void FieldPartyManager_CheckAutomaticBounds(FieldPartyManager *manager)
{
    FieldSystem *system = (FieldSystem *)manager->owner;
    if ((!system || !system->scene_transition.phase) && !manager->bits.unknown_01) {
        FieldAreaContext *area =
            manager->areas[manager->parties[manager->bits.active_party].flags.field_screen];
        if (area && area->unknown_23f0 == 71 && !area->transition.flags.pending &&
            !area->unknown_2402.unknown_00) {
            int black_or_white = 1;
            if (area->brightness != -4096)
                black_or_white = area->brightness == 4096;
            if (!black_or_white) {
                FieldPartyEntity *member = manager->parties[manager->bits.active_party].leader;
                if (member && member->presentation.unknown_15) {
                    int index = FieldPartyEntity_FindPairedBounds(member, area->paired_bounds);
                    if (index != -1) {
                        FieldPairedBoundsRuntime *bounds = &area->paired_bounds[index];
                        switch (bounds->flags.unknown_07_08) {
                        case 0:
                            func_ov000_020802e0(area, manager->bits.active_party, bounds, 1);
                            break;
                        case 1:
                            FieldPartyManager_TransitionThroughBounds(
                                manager, manager->bits.active_party, bounds);
                            break;
                        case 2:
                            break;
                        }
                    }
                }
            }
        }
    }
}
