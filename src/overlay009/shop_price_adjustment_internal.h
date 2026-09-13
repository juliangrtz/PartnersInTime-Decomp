#ifndef PIT_SHOP_PRICE_ADJUSTMENT_INTERNAL_H
#define PIT_SHOP_PRICE_ADJUSTMENT_INTERNAL_H
#include "shop_quantity_internal.h"
#include <game/shop_price_adjustment.h>

struct ShopPriceAdjustmentTask {
    u8 unknown_00[16];
    ShopPriceAdjustmentTask *parent;
    u8 unknown_14[12];
    int phase;
    u8 unknown_24[4];
    union { s32 x; int part; };
    union { s32 y; u32 previous_value; };
    u8 unknown_30[24];
};
typedef char ShopPriceAdjustmentTaskSize[sizeof(ShopPriceAdjustmentTask)==72?1:-1];
extern "C" void func_ov009_0207c6a4(int engine, int tile, u32 x, u32 value, int style);
#define ADJUSTMENT_TASK (*(ShopPriceAdjustmentTask **)(data_ov009_0207ea3c + 0x8bc))


#endif
