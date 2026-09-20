/*
 * Pause list selection sprites (overlay 7, 0x0207EA4C-0x0207F11C).
 *
 * Creates the selected-row cursor and marker, scroll arrows for lists longer
 * than nine rows, and the optional heading for item lists. The heading slides
 * in, follows selection changes and slides out when its controller closes.
 */
#include "pause_scene_internal.h"
#include <game/pause_navigation.h>
#include <game/pause_selection_sprites.h>
#include <game/battle_scene.h>
#include <game/overlay007_party.h>
#include <game/scene_menu_motion.h>
#include <game/menu_equipment.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(void *);
void func_ov005_0206650c(void *);
void func_ov005_02069084(void *, int);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
void *func_ov005_020698dc(int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
u8 func_ov007_020750bc(Overlay7Party *);
int func_ov007_02074b88(Overlay7Party *);
u8 func_ov007_02075400(Overlay7Party *);
void func_ov007_0207f11c(void);
extern u8 data_ov007_02090717[];
}
#define PARTY ((Overlay7Party *)data_ov007_0208e1e4)

static inline void SetPosition(BattleModel *model, int x, int y)
{
    model->animation_offset_x = x;
    model->animation_offset_y = y;
}
extern "C" void PauseList_UpdateItemHeading(MenuEquipmentTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (!WORK.equipment_active && task->state < MENU_EQUIPMENT_CLOSE)
        task->state = MENU_EQUIPMENT_CLOSE;
    switch (task->state) {
    case MENU_EQUIPMENT_ENTER:
        task->heading.y += task->heading.step_y;
        if (task->heading.y >= task->heading.target_y) {
            task->heading.y = task->heading.target_y;
            task->state = MENU_EQUIPMENT_IDLE;
        }
        break;
    case MENU_EQUIPMENT_IDLE:
        if (task->heading.member != data_ov007_02090717[WORK.selected_menu]) {
            task->heading.step_y = (-32768 - task->heading.y) / 8;
            task->counter = 4;
            ++task->state;
        }
        break;
    case MENU_EQUIPMENT_SWITCH_OUT:
        if (task->counter)
            --task->counter;
        else {
            task->heading.y += task->heading.step_y;
            if (task->heading.y <= -32768) {
                task->heading.y = -32768;
                task->heading.member = data_ov007_02090717[WORK.selected_menu];
                task->heading.step_y = (task->heading.target_y - task->heading.y) / 8;
                WORK.equipment_switching = 0;
                task->counter = 4;
                ++task->state;
            }
        }
        break;
    case MENU_EQUIPMENT_SWITCH_IN:
        if (task->counter)
            --task->counter;
        else {
            task->heading.y += task->heading.step_y;
            if (task->heading.y >= task->heading.target_y) {
                task->heading.y = task->heading.target_y;
                task->state = MENU_EQUIPMENT_IDLE;
            }
        }
        break;
    case MENU_EQUIPMENT_CLOSE:
        task->heading.step_y = (-32768 - task->heading.y) / 8;
        ++task->state;
        // The close state moves immediately in the same update.
    case MENU_EQUIPMENT_EXIT:
        task->heading.y += task->heading.step_y;
        if (task->heading.y <= -32768) {
            func_ov005_0206650c(task);
            return;
        }
        break;
    }
    WORK.equipment_heading_x = task->heading.x;
    WORK.equipment_heading_y = task->heading.y;
    SetPosition(model, task->heading.x / 4096, task->heading.y / 4096);
    func_ov005_02069084(model, 7);
}

extern "C" void PauseList_CreateSelectionSprites(PausePageTask *parent, int suppress_heading)
{
    void *resource = func_ov005_020698dc(75);
    if (func_ov007_020750bc(PARTY)) {
        PauseListArrowTask *cursor =
            (PauseListArrowTask *)func_ov005_0206659c(PauseList_UpdateSelectionCursor, 3, 1);
        BattleModel *sprite = Overlay5ResourceA_Attach(cursor, 0, 1);
        func_ov005_02068908(sprite, 0, resource, 1, -1);
        sprite->set_primary_animation(0, 0, 1);
        sprite->flags = (sprite->flags & ~3) | 1;
        cursor->parent = parent;
        if (func_ov007_02074b88(PARTY)) {
            PauseListArrowTask *up = (PauseListArrowTask *)func_ov005_0206659c(
                (void (*)(PauseMenuElement *))PauseList_UpdateScrollArrow, 3, 1);
            BattleModel *upper_sprite = Overlay5ResourceA_Attach(up, 0, 1);
            func_ov005_02068908(upper_sprite, 0, resource, 1, -1);
            upper_sprite->set_primary_animation(1, 0, 1);
            upper_sprite->flags = (upper_sprite->flags & ~3) | 1;
            up->x = (data_ov007_020906f0.x + 128) << 12;
            up->y = (data_ov007_020906f0.y + 28) << 12;
            up->part = 0;
            up->parent = parent;
            PauseListArrowTask *down = (PauseListArrowTask *)func_ov005_0206659c(
                (void (*)(PauseMenuElement *))PauseList_UpdateScrollArrow, 3, 1);
            BattleModel *lower_sprite = Overlay5ResourceA_Attach(down, 0, 1);
            func_ov005_02068908(lower_sprite, 0, resource, 1, -1);
            lower_sprite->flags = (lower_sprite->flags & ~3) | 1;
            lower_sprite->set_primary_animation(2, 0, 1);
            down->x = (data_ov007_020906f0.x + 128) << 12;
            down->y = (data_ov007_020906f0.y + 158) << 12;
            down->part = 1;
            down->parent = parent;
        }
        PauseMenuElement *marker = func_ov005_0206659c(PauseList_UpdateSelectionMarker, 3, 1);
        BattleModel *marker_sprite = Overlay5ResourceA_Attach(marker, 0, 1);
        void *marker_resource = func_ov005_020698dc(78);
        func_ov005_02068908(marker_sprite, 0, marker_resource, 0, -1);
        if (func_ov007_02075400(PARTY) == 1)
            marker_sprite->set_primary_animation(9, 0, 1);
        else
            marker_sprite->set_primary_animation(8, 0, 1);
        marker_sprite->flags = (marker_sprite->flags & ~3) | 2;
    }
    // Each query is a separate native call; do not cache the list kind.
    if (func_ov007_02075400(PARTY) == 0 || func_ov007_02075400(PARTY) == 1) {
        if (!suppress_heading) {
            if (WORK.unknown11f) {
                WORK.equipment_active = 1;
                MenuEquipmentTask *heading = (MenuEquipmentTask *)func_ov005_0206659c(
                    (void (*)(PauseMenuElement *))PauseList_UpdateItemHeading, 5, 1);
                BattleModel *sprite = Overlay5ResourceA_Attach(heading, 0, 1);
                void *heading_resource = func_ov005_020698dc(78);
                func_ov005_02068908(sprite, 0, heading_resource, 0, -1);
                switch (WORK.unknown11f) {
                case 1:
                    sprite->set_primary_animation(0, 0, 1);
                    break;
                case 2:
                    sprite->set_primary_animation(11, 0, 1);
                    break;
                case 3:
                    sprite->set_primary_animation(10, 0, 1);
                    break;
                }
                sprite->flags = (sprite->flags & ~3) | 1;
                heading->heading.member = data_ov007_02090717[WORK.selected_menu];
                heading->heading.x = 0x80000;
                heading->heading.target_y = 0x10000;
                heading->heading.y = -32768;
                heading->heading.step_y = (heading->heading.target_y - heading->heading.y) / 4;
                func_ov007_0207f11c();
            }
        }
    }
}
