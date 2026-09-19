#include "shop_price_adjustment_internal.h"

extern "C" void ShopPriceAdjustment_DrawSymbol(ShopPriceAdjustmentTask *task)
{
    ShopPriceAdjustmentTask *parent = task->parent;
    BattleModel *model = Overlay5ResourceA_Get((ShopRowTask *)task);
    if (!ShopItemPanel_GetCount(data_ov009_0207ea20)) return;
    int x = (parent->x + 73728) / 4096;
    int y = parent->y / 4096;
    model->animation_offset_x = x;
    model->animation_offset_y = y;
    func_ov005_02069084(model, 8);
}

extern "C" void ShopPriceAdjustment_DrawValue(ShopPriceAdjustmentTask *task)
{
    ShopPriceAdjustmentTask *parent = task->parent;
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!ShopItemPanel_GetCount(data_ov009_0207ea20)) return;
    if (!data_ov009_0207ea20->enabled) return;
    u32 value = ((ShopRowPanelInterface *)data_ov009_0207ea20)->get_row_adjustment(
        ShopItemPanel_GetSelection(data_ov009_0207ea20));
    value = QUANTITY * value;
    if (!task->part && task->previous_value != value) {
        task->previous_value = value;
        func_ov009_0207c6a4(0, 664, 26, value, 1);
    }
    int digits = 2;
    while (value >= 10) {
        value /= 10;
        ++digits;
    }
    sprite->x = parent->x + (((6 * digits + 2) << 12) / 2 - 262144);
    sprite->y = parent->y - 106496;
    func_ov005_02069084(sprite, 8);
}

extern "C" void ShopPriceAdjustment_DrawValueMarker(ShopPriceAdjustmentTask *task)
{
    ShopPriceAdjustmentTask *parent = task->parent;
    BattleModel *model = Overlay5ResourceA_Get((ShopRowTask *)task);
    if (!ShopItemPanel_GetCount(data_ov009_0207ea20)) return;
    if (!data_ov009_0207ea20->enabled) return;
    u32 value = ((ShopRowPanelInterface *)data_ov009_0207ea20)->get_row_adjustment(
        ShopItemPanel_GetSelection(data_ov009_0207ea20));
    value = QUANTITY * value;
    int digits = 1;
    while (value >= 10) {
        value /= 10;
        ++digits;
    }
    int x = (parent->x + (-4096 - ((6 * digits + 2) << 12) / 2)) / 4096;
    int y = (parent->y - 69632) / 4096;
    model->animation_offset_x = x;
    model->animation_offset_y = y;
    func_ov005_02069084(model, 8);
}

extern "C" void ShopPriceAdjustment_SetMode(int phase)
{
    if (ADJUSTMENT_TASK) {
        switch (phase) {
        case 0: ADJUSTMENT_TASK->phase = 0; break;
        case 1: ADJUSTMENT_TASK->phase = 100; break;
        case 2: ADJUSTMENT_TASK->phase = 200; break;
        case 3: ADJUSTMENT_TASK->phase = 300; break;
        case 4: ADJUSTMENT_TASK->phase = 400; break;
        }
    }
}
