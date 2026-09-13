#include "shop_navigation_internal.h"

extern "C" {
void ShopMenuOptions_DrawText(ShopRowTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    int x = ShopList_GetRowX(data_ov009_0207ea2c, (u8)task->part) / 4096 - 40;
    int y = ShopList_GetRowY(data_ov009_0207ea2c, (u8)task->part) / 4096 + 2;
    int scale;
    sprite->x = x << 12;
    sprite->y = y << 12;
    scale = ShopList_GetScale(data_ov009_0207ea2c);
    if (scale > 409) {
        func_ov005_020692c0(sprite, 256, (u16)(scale / 16), 0);
        func_ov005_02069084(sprite, (u8)ShopList_GetRowPriority(data_ov009_0207ea2c, (u8)task->part));
    }
}

void ShopMenuOptions_DrawModel(ShopRowTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    int x = ShopList_GetRowX(data_ov009_0207ea2c, (u8)task->part) / 4096 - 56;
    int y = ShopList_GetRowY(data_ov009_0207ea2c, (u8)task->part);
    int scale;
    model->animation_offset_x = x;
    model->animation_offset_y = y / 4096 + 8;
    scale = ShopList_GetScale(data_ov009_0207ea2c);
    if (scale > 409) {
        model->scale_x = 256;
        model->scale_y = scale / 16;
        func_ov005_02069084(model, (u8)(ShopList_GetRowPriority(data_ov009_0207ea2c, (u8)task->part) - 3));
    }
}
}
