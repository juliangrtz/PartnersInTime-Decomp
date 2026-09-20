/* Equipped-member markers on the animated buying list (overlay 9,
 * 0x02070AD0-0x02070CFC). Paired badge wearers share their row marker. */

#include "shop_rows_internal.h"
extern "C" {
#include <game/save_data.h>
extern u8 data_ov009_0207eace[4];
}
// Preserve the native member stride before the fixed save-data offset.
struct SavedMemberView {
    u8 prefix[0x3f8];
    SavePartyMember member;
};
#define MEMBER(i) (((SavedMemberView *)(gSaveData + sizeof(SavePartyMember) * (u8)(i)))->member)
extern "C" void ShopBuyMarker_Update(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int equipped;
    if (ShopBuyPanel_GetCategory((ShopBuyPanel *)data_ov009_0207ea34) == 2)
        equipped =
            (u16)(MEMBER(task->part).experience_to_next_level.equipment.clothing_id + 0x4000);
    else
        equipped = (u16)(MEMBER(task->part).badge_id + 0x3000);
    int rows = ShopItemPanel_GetCount(data_ov009_0207ea34);
    if (rows > ShopItemPanel_GetVisibleRows(data_ov009_0207ea34))
        rows = ShopItemPanel_GetVisibleRows(data_ov009_0207ea34);
    int row = 0;
    for (; row < rows; ++row)
        if (equipped == ShopItemPanel_GetRowItem(data_ov009_0207ea34, (u16)row))
            break;
    if (row != rows) {
        int adjustment = 0;
        if (ShopBuyPanel_GetCategory((ShopBuyPanel *)data_ov009_0207ea34) == 3) {
            s8 other;
            if (task->part < 2)
                other = task->part + 2;
            else
                other = task->part - 2;
            if (!data_ov009_0207eace[other] || equipped != MEMBER(other).badge_id + 0x3000)
                adjustment = 1 - sprite->offset_y / 4096;
        }
        int x = ShopList_GetRowX(data_ov009_0207ea2c, (u8)row) - 0x40000;
        int y = ShopList_GetRowY(data_ov009_0207ea2c, (u8)row) + (adjustment << 12);
        int scale = ShopList_GetScale(data_ov009_0207ea2c);
        sprite->x = x - 0x12000;
        sprite->y = y + 0x2000;
        // Hidden while the list is almost collapsed; rows retain their latest position.
        if (scale > 409) {
            func_ov005_020692c0(sprite, 256, (u16)(scale / 16), 0);
            func_ov005_02069084(sprite, (u8)ShopList_GetRowPriority(data_ov009_0207ea2c, (u8)row));
        }
    }
}
