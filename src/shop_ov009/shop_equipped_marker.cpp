#include "shop_rows_internal.h"
extern "C" {
#include <game/save_data.h>
extern ShopItemPanel *data_ov009_0207ea24;
extern u8 data_ov009_0207eace[4];
}
struct ShopEquippedMarkerTask {
    u8 prefix[40];
    int member, category;
    u8 tail[24];
};
typedef char MarkerTaskSize[sizeof(ShopEquippedMarkerTask) == 72 ? 1 : -1];
struct SavedMemberView {
    u8 prefix[0x3f8];
    SavePartyMember member;
};
#define MEMBER(i) (((SavedMemberView *)(gSaveData + sizeof(SavePartyMember) * (u8)(i)))->member)
extern "C" void ShopEquippedMarker_Update(ShopEquippedMarkerTask *task) {
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int equipped;
    if (task->category)
        equipped = (u16)(MEMBER(task->member).badge_id + 0x3000);
    else
        equipped =
            (u16)(MEMBER(task->member).experience_to_next_level.equipment.clothing_id + 0x4000);
    int rows = ShopItemPanel_GetCount(data_ov009_0207ea24);
    if (rows > ShopItemPanel_GetVisibleRows(data_ov009_0207ea24))
        rows = ShopItemPanel_GetVisibleRows(data_ov009_0207ea24);
    int row = 0;
    for (; row < rows; ++row)
        if (equipped == ShopItemPanel_GetRowItem(data_ov009_0207ea24, (u16)row))
            break;
    if (row != rows) {
        int adjustment = 0;
        if (task->category) {
            s8 other;
            if (task->member < 2)
                other = task->member + 2;
            else
                other = task->member - 2;
            if (!data_ov009_0207eace[other] || equipped != MEMBER(other).badge_id + 0x3000)
                adjustment = 1 - sprite->offset_y / 4096;
        }
        int x = ShopItemPanel_GetX(data_ov009_0207ea24);
        int y = ShopItemPanel_GetY(data_ov009_0207ea24) + (adjustment << 12) + (row << 16);
        sprite->x = x - 0x12000;
        sprite->y = y + 0x2000;
        func_ov005_02069084(sprite, 60);
    }
}
