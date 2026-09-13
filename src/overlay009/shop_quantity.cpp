#include "shop_quantity_internal.h"

extern "C" void ShopQuantity_DrawArrow(ShopQuantityTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get((ShopRowTask *)task);
    if (!VISIBLE) {
        func_ov005_0206650c(task);
        return;
    }
    if (SCALE <= 1024) return;
    int y;
    if (!task->part) {
        y = 84;
        int unavailable = 0;
        u16 available = ((ShopRowPanelInterface *)data_ov009_0207ea20)->get_row_quantity(
            ShopItemPanel_GetSelection(data_ov009_0207ea20), 1);
        if (!task->selling) {
            u16 maximum = func_ov009_0207ddac(ShopItemPanel_GetRowItem(
                data_ov009_0207ea20, ShopItemPanel_GetSelection(data_ov009_0207ea20)));
            if (available + QUANTITY + 1 > maximum) unavailable = 1;
            u32 currency;
            if (!SPECIAL && data_ov009_0207ea3c[0x98] == 2)
                currency = *(u16 *)(gSaveData + 1164);
            else
                currency = *(u32 *)(gSaveData + 1160);
            if (currency < PRICE * (QUANTITY + 1)) unavailable = 1;
        } else {
            if (available < QUANTITY + 1) unavailable = 1;
        }
        if (unavailable) {
            model->animation_offset_x = -128;
            model->animation_offset_y = -128;
            func_ov005_02069084(model, 10);
            return;
        }
    } else {
        y = 106;
        if (QUANTITY == 1) {
            model->animation_offset_x = -128;
            model->animation_offset_y = -128;
            func_ov005_02069084(model, 10);
            return;
        }
    }
    int position = (y - 96) * SCALE + 393216;
    model->animation_offset_x = 168;
    model->animation_offset_y = position / 4096;
    int scale = SCALE;
    model->scale_x = 256;
    model->scale_y = scale / 16;
    func_ov005_02069084(model, 10);
}

extern "C" void ShopQuantity_DrawSlidingSprite(ShopQuantityTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!VISIBLE) {
        func_ov005_0206650c(task);
        return;
    }
    if (SCALE <= 1024) return;
    sprite->offset_y = -4 * (4096 - SCALE);
    func_ov005_020692c0(sprite, 256, (u16)(SCALE / 16), 0);
    func_ov005_02069084(sprite, 10);
}

extern "C" void ShopQuantity_DrawSprite(ShopQuantityTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!VISIBLE) {
        func_ov005_0206650c(task);
        return;
    }
    if (SCALE <= 1024) return;
    func_ov005_020692c0(sprite, 256, (u16)(SCALE / 16), 0);
    func_ov005_02069084(sprite, 10);
}

extern "C" void ShopQuantity_DrawPositionedSprite(ShopQuantityTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!VISIBLE) {
        func_ov005_0206650c(task);
        return;
    }
    if (SCALE <= 2560) return;
    sprite->offset_y = -6 * (4096 - SCALE);
    sprite->y = task->y * SCALE + 393216;
    func_ov005_020692c0(sprite, 256, (u16)(SCALE / 16), 0);
    func_ov005_02069084(sprite, 11);
}

extern "C" void ShopQuantity_DrawItemName(u16 item)
{
    func_ov009_0207459c(data_ov009_0207ea30, DISPLAY_ENGINE_MAIN, 1280, 0,
                       item, 4, 1, 32, 6, 0, (u8)QUANTITY);
    if (!SPECIAL) {
        u8 *source = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + 1280;
        u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_SUB) + 1280;
        MIi_CpuCopy16(source, destination, 1152);
    }
}

extern "C" void ShopQuantity_CopySelectedIcon(void)
{
    u8 *source = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN);
    source += 32 * (ShopItemPanel_GetSelectedTile(data_ov009_0207ea20) + 32);
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + 2304;
    MIi_CpuCopy16(source, destination, 128);
}

extern "C" void ShopQuantity_DrawDigit(ShopQuantityTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!VISIBLE) {
        func_ov005_0206650c(task);
        return;
    }
    if (SCALE <= 2048) return;
    if (!task->part && task->previous_quantity != QUANTITY &&
        (task->previous_quantity == 1 || QUANTITY == 1))
        ShopQuantity_DrawItemName((u16)task->item);
    int digit;
    if (!task->part) digit = QUANTITY % 10;
    else digit = QUANTITY / 10;
    task->previous_quantity = QUANTITY;
    if (!digit && task->part) return;
    sprite->attributes_1 = (sprite->attributes_1 & ~0x3ffu) |
                           ((task->first_tile + digit) & 0x3ff);
    sprite->offset_y = -4 * (4096 - SCALE);
    func_ov005_020692c0(sprite, 256, (u16)(SCALE / 16), 0);
    func_ov005_02069084(sprite, 10);
}

extern "C" void ShopQuantity_DrawTotal(ShopQuantityTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!VISIBLE) {
        func_ov005_0206650c(task);
        return;
    }
    if (SCALE <= 1024) return;
    if (!task->part) {
        int total = PRICE * QUANTITY;
        if (task->previous_total != total) {
            task->previous_total = total;
            func_ov009_0207c8f0(648, 13, total, 6);
        }
    }
    func_ov005_020692c0(sprite, 256, (u16)(SCALE / 16), 0);
    func_ov005_02069084(sprite, 10);
}
