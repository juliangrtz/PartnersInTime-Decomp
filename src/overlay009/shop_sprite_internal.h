#ifndef PIT_SHOP_SPRITE_INTERNAL_H
#define PIT_SHOP_SPRITE_INTERNAL_H
#include <nitro.h>

/* The first 64 bytes of the attached sprite, through its affine matrix. */
struct ShopRowSprite {
    u8 unknown_00[20];
    u32 attributes_0;
    union {
        u32 attributes_1;
        struct {
            u32 tile : 10, size : 2, palette : 4, unknown_16_31 : 16;
        } bits;
    };
    s32 x, y, offset_x, offset_y;
    s16 origin_x, origin_y;
    u8 unknown_30[8];
    s16 matrix[4];
};
typedef char ShopRowSpriteSizeCheck[sizeof(ShopRowSprite) == 64 ? 1 : -1];
#endif
