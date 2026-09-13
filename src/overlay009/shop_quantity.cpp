#include "shop_quantity_internal.h"

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
