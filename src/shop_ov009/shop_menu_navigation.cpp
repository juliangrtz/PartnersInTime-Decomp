#include "shop_navigation_internal.h"

extern "C" {
void ShopMenuNavigation_DrawCursor(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    int x = ShopList_GetRowX(data_ov009_0207ea2c, (u8)(2 * ((s8 *)data_ov009_0207ea3c)[0x9f])) / 4096 - 68;
    int y = ShopList_GetRowY(data_ov009_0207ea2c, (u8)(2 * ((s8 *)data_ov009_0207ea3c)[0x9f]));
    model->animation_offset_x = x;
    model->animation_offset_y = y / 4096 + 8;
    func_ov005_02069084(model, 8);
}

void ShopMenuNavigation_Create(void)
{
    void *resource = func_ov005_020698dc(0);
    ShopRowTask *task = func_ov005_0206659c(ShopMenuNavigation_DrawCursor, 3, 1);
    BattleModel *model = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(model, 0, resource, 1, -1);
    model->set_primary_animation(0, 0, 1);
    model->flag_bits.unknown_00_01 = 2;
}

void ShopMenuNavigation_Clear(void)
{
    func_ov005_020663d8(3);
    func_ov005_020663d8(5);
}
}
