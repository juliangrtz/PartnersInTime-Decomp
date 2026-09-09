extern "C" {
#include <nitro/fx.h>
}
#include <game/field_party_manager.h>
#include <game/field_room_transition.h>
extern "C" {
void func_ov000_0209f644(FieldPartyManager *, int, int, int, int, int, int, u8, int, int, u8, u8);
}
extern "C" void FieldPartyManager_TransitionThroughBounds(FieldPartyManager *manager, int side,
                                                          const FieldPairedBoundsRuntime *record)
{
    fx32 x, y;
    unsigned direction = record->flags.mode;
    /* Inverting the one-bit resource flag preserves the native entry-slide policy. */
    int entry_slide = record->flags.unknown_06 ^ 1;
    x = record->bounds[1].min_x;
    y = record->bounds[1].min_y;
    fx32 z = record->bounds[1].z;
    if (record->flags.unknown_02) {
        FieldRuntimeEntity *leader = &manager->parties[side].members[0]->entity;
        switch (direction) {
        case 0:
        case 4: {
            fx32 minimum = record->bounds[0].min_x - leader->navigation_min_x;
            fx32 maximum = record->bounds[0].max_x - leader->navigation_max_x;
            x -= leader->navigation_min_x;
            fx32 target_maximum = record->bounds[1].max_x - leader->navigation_max_x;
            x += FX_Div(((s64)(leader->position_x - minimum) * (target_maximum - x) + 2048) >> 12,
                        maximum - minimum);
            break;
        }
        case 2:
        case 6: {
            fx32 minimum = record->bounds[0].min_y - leader->navigation_min_y;
            fx32 maximum = record->bounds[0].max_y - leader->navigation_max_y;
            y -= leader->navigation_min_y;
            fx32 target_maximum = record->bounds[1].max_y - leader->navigation_max_y;
            y += FX_Div(((s64)(leader->position_y - minimum) * (target_maximum - y) + 2048) >> 12,
                        maximum - minimum);
            break;
        }
        case 1:
        case 3:
        case 5:
        case 7:
        default:
            break;
        }
    }
    int screen = manager->parties[side].flags.field_screen;
    func_ov000_0209f644(manager, side, record->id, x / 4096, y / 4096, z / 4096, direction, entry_slide != 0,
                        -1, -1, screen != 0, screen != 1);
}
extern "C" void FieldPartyManager_ApplyPendingTransition(FieldPartyManager *manager)
{
    func_ov000_0209f644(manager, manager->pending_transition.side, manager->pending_transition.room,
                        manager->pending_transition.x, manager->pending_transition.y,
                        manager->pending_transition.z, manager->pending_transition.direction,
                        manager->pending_transition.entry_slide != 0, manager->pending_transition.script,
                        manager->pending_transition.bgm, manager->pending_transition.option_9 != 0,
                        manager->pending_transition.option_10 != 0);
    manager->pending_transition.active = 0;
}
extern "C" void FieldPartyManager_CheckRegion71Bounds(FieldPartyManager *manager, int side)
{
    if (manager->bits.unknown_13 == 1 && manager->parties[side].flags.field_screen == 1 &&
        !manager->parties[side].backup.active) {
        FieldAreaContext *field = manager->areas[1];
        if (field && field->unknown_23f0 == 71) {
            const FieldPairedBoundsRuntime *record = field->paired_bounds;
            if (record) {
                while (!record->flags.last) {
                    if (record->flags.unknown_07_08 == 2) {
                        FieldPartyManager_QueuePairedBounds(manager, side, record);
                        manager->bits.unknown_16 = 1;
                        return;
                    }
                    ++record;
                }
            }
        }
    }
}
extern "C" void FieldPartyManager_QueuePairedBounds(FieldPartyManager *manager, int side,
                                                    const FieldPairedBoundsRuntime *record)
{
    manager->pending_bounds.active = 1;
    manager->pending_bounds.side = (u16)side;
    manager->pending_bounds_record = record;
    FieldArea_QueueRoomChange(manager->areas[1], data_02048f18[manager->areas[0]->room_id].paired_room, -1, -1, 1);
}
