#include "shop_currency_internal.h"

extern "C" void ShopDisplay_Init(void)
{
    func_ov009_0207b854();
    ShopHelpText_InitBackground();
    if (!data_ov009_0207ea3c[0x99]) {
        if (data_ov009_0207ea3c[0x98])
            func_ov009_0207a9b8();
        func_ov009_0207865c(0);
    } else {
        func_ov009_0207865c(1);
        func_ov009_0207bbe0();
    }
}

extern "C" void ShopCurrency_Draw(ShopCurrencyTask *task)
{
    ShopRowSprite *sprite = Overlay5ResourceB_Get(task);
    if (!task->part) {
        int value;
        if (!data_ov009_0207ea3c[0x99] && data_ov009_0207ea3c[0x98] == 2) {
            value = *(u16 *)(gSaveData + 1164);
            if (value > 999) value = 999;
        } else {
            value = *(s32 *)(gSaveData + 1160);
            if (value > 999999) value = 999999;
        }
        if (task->value != value) {
            task->current = task->value << 8;
            task->target = value << 8;
            task->value = value;
            int delta = task->target - task->current;
            if (delta < 0) delta = -delta;
            if (delta >= 8192) task->duration = 32;
            else if (delta >= 4096) task->duration = 16;
            else if (delta >= 2048) task->duration = 8;
            else task->duration = 1;
            task->step = (task->target - task->current) / task->duration;
            task->frame = 0;
            task->animating = 1;
        }
        if (task->animating) {
            func_ov005_02069bcc(241, 0, 4, 128);
            task->current += task->step;
            if (++task->frame >= task->duration) {
                task->current = task->target;
                task->animating = 0;
            }
            func_ov009_0207c8f0(632, 13, task->current / 256, 6);
        }
    }
    sprite->y = *(s32 *)(data_ov009_0207ea3c + 0x8c8) + 561152;
    func_ov005_02069084(sprite, 60);
}
