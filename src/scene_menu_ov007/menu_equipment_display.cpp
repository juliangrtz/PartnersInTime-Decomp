#include "pause_scene_internal.h"
#include <game/battle_scene.h>
#include <game/menu_number.h>
extern "C" {
#include <game/menu_equipment.h>
#include <game/menu_item_effect.h>
void func_ov005_0206650c(MenuEquipmentTask *);
BattleModel *Overlay5ResourceA_Get(MenuEquipmentTask *);
MenuItemEffectSprite *Overlay5ResourceB_Get(MenuEquipmentTask *);
void func_ov005_02069084(void *, int);
}
/* Applying the member stride first preserves the native save-field addressing.
   For every member, this prefix remains inside the live save allocation. */
struct EquipmentSaveMemberView {
    u8 unknown_000[1016];
    SavePartyMember member;
};
typedef char EquipmentSaveMemberViewSize[sizeof(EquipmentSaveMemberView) == 1052 ? 1 : -1];

extern "C" {
extern HighlightItemView data_ov007_020907f0;
extern s8 data_ov007_020908b8[5];
int func_ov007_02075400(void *);
int func_ov007_02074b44(void *, int, int, u32);
void func_ov007_02074a00(void *, int, s16 *, s16 *, s16 *, s16 *, s16 *);
}

extern "C" void MenuEquipment_UpdateStatRow(MenuEquipmentTask *task)
{
    int member;
    u32 current, preview;
    int available, x;
    MenuEquipmentTask *parent = task->parent;
    member = (u8)parent->panel.member;
    s16 hp = 0, power = 0, defense = 0, speed = 0, stache = 0;
    if (parent->panel.closing) {
        func_ov005_0206650c(task);
        return;
    }
    int kind = func_ov007_02075400(data_ov007_0208e1e4);
    available = func_ov007_02074b44(data_ov007_0208e1e4, kind,
                                  data_ov007_020907f0.item, member);
    func_ov007_02074a00(data_ov007_0208e1e4, data_ov007_020907f0.item,
                       &hp, &power, &defense, &speed, &stache);
    int stat = task->row.stat;
    switch (stat) {
    case 0: {
        const EquipmentSaveMemberView *party =
            (const EquipmentSaveMemberView *)(gSaveData + sizeof(SavePartyMember) * member);
        preview = (u16)(party->member.base_max_hp + hp);
        current = party->member.max_hp;
        break;
    }
    case 1: {
        const EquipmentSaveMemberView *party =
            (const EquipmentSaveMemberView *)(gSaveData + sizeof(SavePartyMember) * member);
        preview = (u16)(party->member.base_power + power);
        current = party->member.power;
        break;
    }
    case 2: {
        const EquipmentSaveMemberView *party =
            (const EquipmentSaveMemberView *)(gSaveData + sizeof(SavePartyMember) * member);
        preview = (u16)(party->member.base_defense + defense);
        current = party->member.defense;
        break;
    }
    case 3: {
        const EquipmentSaveMemberView *party =
            (const EquipmentSaveMemberView *)(gSaveData + sizeof(SavePartyMember) * member);
        preview = (u16)(party->member.base_speed + speed);
        current = party->member.speed;
        break;
    }
    case 4: {
        const EquipmentSaveMemberView *party =
            (const EquipmentSaveMemberView *)(gSaveData + sizeof(SavePartyMember) * member);
        preview = (u16)(party->member.base_stache + stache);
        current = party->member.stache;
        break;
    }
    }
    if (!available)
        preview = 65535;
    if (task->row.current == current && task->row.preview == preview)
        return;
    int wide = 0;
    if (preview != current)
        wide = 1;
    if (task->row.current != current)
        MenuNumber_DrawValue(16 * stat + 132, 10, current, 0);
    task->row.current = current;
    task->row.preview = preview;
    if (!available) {
        data_ov007_020908b8[task->row.stat] = MENU_EQUIPMENT_UNAVAILABLE;
        MenuNumber_DrawUnavailable(8 * (2 * task->row.stat + 1) + 132, 10);
        return;
    }
    if (preview == current) {
        wide = 0;
        data_ov007_020908b8[task->row.stat] = MENU_EQUIPMENT_SAME;
        x = 10;
    }
    if (preview > current) {
        wide = 1;
        data_ov007_020908b8[task->row.stat] = MENU_EQUIPMENT_HIGHER;
        x = 4;
    }
    if (preview < current) {
        data_ov007_020908b8[task->row.stat] = MENU_EQUIPMENT_LOWER;
        wide = 1;
        x = 4;
    }
    MenuNumber_DrawValue(8 * (2 * task->row.stat + 1) + 132, (u16)x, preview, wide);
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
