#include "pause_scene_internal.h"
#include <game/battle_scene.h>
extern "C" {
#include <game/menu_equipment.h>
#include <game/menu_item_effect.h>
void func_ov005_0206650c(MenuEquipmentTask *);
BattleModel *Overlay5ResourceA_Get(MenuEquipmentTask *);
MenuItemEffectSprite *Overlay5ResourceB_Get(MenuEquipmentTask *);
void func_ov005_02069084(void *, int);
}
static inline void SetPosition(BattleModel *sprite, int x, int y)
{
    sprite->animation_offset_x = x;
    sprite->animation_offset_y = y;
}
extern "C" void MenuEquipment_UpdateHeading(MenuEquipmentTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
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
        if (task->heading.member != WORK.selected[0]) {
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
                task->heading.member = WORK.selected[0];
                sprite->set_primary_animation((u8)(WORK.selected[0] + 1), 0, 1);
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
    SetPosition(sprite, task->heading.x / 4096, task->heading.y / 4096);
    func_ov005_02069084(sprite, 15);
}
extern "C" void MenuEquipment_UpdateStatsPanel(MenuEquipmentTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    for (int i = 0; i < 3; ++i)
        WORK.equipment_frames[i] = -1;
    if (!WORK.equipment_active && task->state < MENU_EQUIPMENT_CLOSE)
        task->state = MENU_EQUIPMENT_CLOSE;
    switch (task->state) {
    case MENU_EQUIPMENT_ENTER:
        task->panel.velocity += task->panel.acceleration;
        task->panel.x += task->panel.velocity;
        if (task->panel.x >= task->panel.target) {
            task->panel.x = task->panel.target;
            task->state = MENU_EQUIPMENT_IDLE;
        }
        break;
    case MENU_EQUIPMENT_IDLE:
        if (task->panel.member == WORK.selected[0])
            break;
        task->panel.velocity = 0;
        ++task->state;
    case MENU_EQUIPMENT_SWITCH_OUT:
        task->panel.velocity -= 8192;
        task->panel.x += task->panel.velocity;
        if (task->panel.x <= -393216) {
            task->panel.member = WORK.selected[0];
            task->panel.velocity = 0;
            ++task->state;
        }
        break;
    case MENU_EQUIPMENT_SWITCH_IN:
        task->panel.velocity += 8192;
        task->panel.x += task->panel.velocity;
        if (task->panel.x >= task->panel.target) {
            task->panel.x = task->panel.target;
            task->state = MENU_EQUIPMENT_IDLE;
        }
        break;
    case MENU_EQUIPMENT_CLOSE:
        task->panel.velocity = 0;
        ++task->state;
    case MENU_EQUIPMENT_EXIT:
        task->panel.velocity -= 8192;
        task->panel.x += task->panel.velocity;
        if (task->panel.x <= -393216) {
            task->panel.closing = 1;
            func_ov005_0206650c(task);
            return;
        }
        break;
    }
    SetPosition(sprite, task->panel.x / 4096, task->panel.y / 4096);
    func_ov005_02069084(sprite, 15);
}
