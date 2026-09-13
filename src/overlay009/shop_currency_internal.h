#ifndef PIT_SHOP_CURRENCY_INTERNAL_H
#define PIT_SHOP_CURRENCY_INTERNAL_H
#include "shop_rows_internal.h"
#include <game/shop_graphics.h>
extern "C" {
#include <game/save_data.h>
extern u8 data_ov009_0207ea3c[];
void func_ov005_02069bcc(int sound, u16 delay, u16 wait, u16 priority);
void func_ov009_0207b854(void);
void ShopHelpText_InitBackground(void);
void func_ov009_0207a9b8(void);
void func_ov009_0207865c(int);
void func_ov009_0207bbe0(void);
}

struct ShopCurrencyTask {
    u8 unknown_00[32];
    int animating, frame, part, value;
    s32 current, target;
    int duration;
    s32 step;
    u8 unknown_40[8];
};
typedef char ShopCurrencyTaskSizeCheck[sizeof(ShopCurrencyTask) == 72 ? 1 : -1];
#endif
