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
extern "C" void MenuEquipment_UpdateNumberStrip(MenuEquipmentTask *task)
{
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    MenuEquipmentTask *parent = task->parent;
    if (parent->panel.closing) {
        func_ov005_0206650c(task);
        return;
    }
    sprite->x = parent->panel.x;
    sprite->y = parent->panel.y;
    func_ov005_02069084(sprite, 12);
}
extern "C" void MenuEquipment_UpdateComparisonArrow(MenuEquipmentTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    MenuEquipmentTask *parent = task->parent;
    if (parent->panel.closing) {
        func_ov005_0206650c(task);
        return;
    }
    /* Every arrow of one comparison kind shares the frame selected first this update. */
    int animation = WORK.equipment_comparison[task->row.stat];
    if (animation < 0)
        return;
    int x = parent->panel.x + (task->row.x << 12), y = parent->panel.y + (task->row.y << 12);
    if (task->row.animation != animation) {
        task->row.animation = animation;
        int frame = WORK.equipment_frames[task->row.animation];
        if (frame == -1)
            sprite->set_primary_animation(task->row.animation, 0, 1);
        else
            sprite->set_primary_animation(task->row.animation, frame, 1);
    } else {
        if (WORK.equipment_frames[task->row.animation] == -1)
            WORK.equipment_frames[task->row.animation] = sprite->property_056;
        else
            sprite->set_primary_animation(task->row.animation, WORK.equipment_frames[task->row.animation], 1);
    }
    sprite->animation_offset_x = x / 4096;
    sprite->animation_offset_y = y / 4096;
    func_ov005_02069084(sprite, 12);
}

extern "C" {
BattleModel *Overlay5ResourceA_Attach(MenuEquipmentTask *, BattleModel *, int);
void *func_ov005_020698dc(int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void func_ov007_0207f11c(void);
}
extern "C" void MenuEquipment_CreateHeading(void)
{
    BattleModel *sprite;
    MenuEquipmentTask *task = (MenuEquipmentTask *)func_ov005_0206659c(
        (void (*)(PauseMenuElement *))MenuEquipment_UpdateHeading, 5, 1);
    sprite = Overlay5ResourceA_Attach(task, 0, 1);
    void *resource = func_ov005_020698dc(78);
    func_ov005_02068908(sprite, 0, resource, 0, -1);
    if (WORK.party_count == 1)
        sprite->set_primary_animation(7, 0, 1);
    else
        sprite->set_primary_animation((u8)(WORK.selected[0] + 1), 0, 1);
    sprite->flags = (sprite->flags & ~3) | 1;
    task->heading.x = 524288;
    task->heading.target_y = 65536;
    task->heading.y = -32768;
    task->heading.step_y = (task->heading.target_y - task->heading.y) / 8;
    task->heading.member = WORK.selected[0];
    WORK.equipment_active = 1;
    if (WORK.party_count > 1)
        func_ov007_0207f11c();
}
