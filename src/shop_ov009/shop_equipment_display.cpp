/* Equipment description and attached sprite updates in the shop comparison panel. */
#include "shop_rows_internal.h"
#include <game/shop_equipment_display.h>
extern "C" {
#include <game/save_state_transfer.h>
/* Prefix of the 2492-byte shop workspace. Selection has both signed
 * comparisons and an unsigned party-array index in the native callbacks. */
struct ShopEquipmentWork {
    u8 unknown_00[0x8d];
    union {
        s8 member;
        u8 member_index;
    };
    u8 unknown_8e[0xb1 - 0x8e];
    s8 category;
    u8 unknown_b2[0x8b6 - 0xb2];
    u8 blend, phase;
};
extern ShopEquipmentWork data_ov009_0207ea3c;
extern ShopText *data_ov009_0207ea30;
void func_ov005_0206650c(void *);
void func_ov009_0207459c(ShopText *, DisplayEngine, int, int, u16, int, int, int, u8, u8, u8);
}
struct ShopEquipmentDisplayParent {
    u8 unknown_00[44];
    s32 x, y;
    s16 offset_x, offset_y;
    u8 unknown_38[4];
    int active;
    u8 unknown_40[8];
};
struct ShopEquipmentDisplayTask {
    u8 unknown_00[16];
    ShopEquipmentDisplayParent *parent;
    u8 unknown_14[16];
    int frames;
    union {
        int cached_item;
        int sub_screen;
    };
    int x, y, latched;
    u8 unknown_38[16];
};
typedef char EquipmentWorkPrefixSize[sizeof(ShopEquipmentWork) == 0x8b8 ? 1 : -1];
typedef char DisplayParentSize[sizeof(ShopEquipmentDisplayParent) == 72 ? 1 : -1];
typedef char DisplayTaskSize[sizeof(ShopEquipmentDisplayTask) == 72 ? 1 : -1];
struct ShopEquipmentDescriptionSpriteTask {
    u8 unknown_00[16];
    ShopEquipmentDisplayTask *owner;
    u8 unknown_14[16];
    int frames;
    u8 unknown_28[32];
};
typedef char DescriptionSpriteTaskSize[sizeof(ShopEquipmentDescriptionSpriteTask) == 72 ? 1 : -1];

#define WORK data_ov009_0207ea3c
extern "C" void ShopPanel_DrawAttachedSprite(ShopEquipmentDisplayTask *task)
{
    ShopEquipmentDisplayParent *parent = task->parent;
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (WORK.phase > 1) {
        func_ov005_0206650c(task);
        return;
    }
    sprite->x = (parent->x + 16 * parent->offset_x) / 4096;
    sprite->y = (parent->y + 16 * parent->offset_y) / 4096;
    sprite->x += task->x;
    sprite->y += task->y;
    if (task->sub_screen)
        sprite->y += 240;
    sprite->x <<= 12;
    sprite->y <<= 12;
    if (WORK.blend)
        sprite->attributes_0 = (sprite->attributes_0 & ~0xc00) | 0x400;
    else
        sprite->attributes_0 &= ~0xc00;
    if (!WORK.phase)
        func_ov005_02069084(sprite, 5);
    else
        func_ov005_02069084(sprite, 59);
}

extern "C" void ShopEquipment_UpdateDescription(ShopEquipmentDisplayTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get((ShopRowTask *)task);
    if (WORK.phase) {
        func_ov005_0206650c(task);
        return;
    }
    if (!task->parent->active || !task->latched) {
        if (WORK.member < 2)
            task->y = 0x28000;
        else
            task->y = 0x80000;
    }
    if (task->frames) {
        --task->frames;
    } else {
        if (WORK.member & 1)
            model->set_primary_animation(6, 0, 1);
        else
            model->set_primary_animation(5, 0, 1);
        // Read the task position before writing the attached model.
        int y = task->y / 4096;
        model->animation_offset_x = 128;
        model->animation_offset_y = 176 - y;
        func_ov005_02069084(model, 22);
        u16 item;
        if (WORK.category)
            item = (u16)(((SaveLiveTransferView *)gSaveData)->party[WORK.member_index].badge_id +
                         0x3000);
        else
            item = (u16)(((SaveLiveTransferView *)gSaveData)
                             ->party[WORK.member_index]
                             .experience_to_next_level.equipment.clothing_id +
                         0x4000);
        if (!task->parent->active)
            task->latched = 1;
        if (!task->parent->active || !task->latched) {
            if (task->cached_item != item) {
                task->cached_item = item;
                func_ov009_0207459c(data_ov009_0207ea30, DISPLAY_ENGINE_SUB, 10880, 1, item, 7, 2,
                                    56, 0, 0, 0);
            }
        }
    }
}

extern "C" void ShopEquipment_DrawDescriptionStrip(ShopEquipmentDescriptionSpriteTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (WORK.phase) {
        func_ov005_0206650c(task);
        return;
    }
    if (task->frames) {
        --task->frames;
    } else {
        ShopEquipmentDisplayTask *owner = task->owner;
        if (!owner->parent->active || !owner->latched) {
            sprite->x = owner->x;
            sprite->y = 0xa6000 - task->owner->y;
        }
        func_ov005_02069084(sprite, 20);
    }
}
