#ifndef PIT_SHOP_PARTY_INTERNAL_H
#define PIT_SHOP_PARTY_INTERNAL_H
#include "shop_rows_internal.h"
#include <game/shop_graphics.h>

struct ShopPartyMotionTask {
    u8 unknown_00[32];
    int mirrored;
    u8 unknown_24[4];
    s32 x, y, offset_x;
    u8 unknown_34[12];
    int visible;
    u8 unknown_44[4];
};
typedef char ShopPartyMotionTaskSizeCheck[sizeof(ShopPartyMotionTask) == 72 ? 1 : -1];
extern "C" {
extern u8 data_ov009_0207ea3c[];
}
#endif
