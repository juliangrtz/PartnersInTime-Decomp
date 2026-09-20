/* Shop party-selection queries, overlay 9, 0x0206C1D8-0x0206C2C4. */
#include "shop_scene_internal.h"
extern "C" {
extern u32 data_ov009_0207e064[2][2];
extern u8 data_ov009_0207eaca[4];
/* Keep row-major first-match behavior and the (0, 0) fallback. */
void ShopScene_GetMemberGridPosition(int member, int *column, int *row)
{
    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            if (member == data_ov009_0207e064[y][x]) {
                *column = x;
                *row = y;
                return;
            }
        }
    }
    *column = 0;
    *row = 0;
}

int ShopScene_CanSelectMember(int column, int row)
{
    /* The grid maps to party IDs; only the low byte selects the presence flag. */
    int member = (u8)data_ov009_0207e064[row][column];
    if (!data_ov009_0207eaca[member])
        return 0;
    /* This scene mode bypasses the selected item's equipment restriction. */
    if (!data_ov009_0207ea3c.unknown_9a[1]) {
        if (!ShopInventory_CanEquip(ShopItemPanel_GetRowItem(
                (ShopItemPanel *)data_ov009_0207ea34,
                ShopItemPanel_GetSelection((ShopItemPanel *)data_ov009_0207ea34)), member))
            return 0;
    }
    return 1;
}
}
