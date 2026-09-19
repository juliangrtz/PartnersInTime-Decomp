#include "shop_graphics_internal.h"

void ShopBitmap_DrawValue(int x, int y, u32 value)
{
    int started = 0;
    int divisor = 100;
    int digit_index;
    func_ov009_0207c558(x, y, 20, 12);
    for (digit_index = 0; digit_index < 3; ++digit_index, x += 6) {
        int digit = value / divisor;
        if (digit || started) {
            started = 1;
            func_ov009_0207c464(x, y - 4, SHOP_BITMAP_GLYPHS + (digit << 8),
                              8, 15, 48, 1);
        }
        value %= divisor;
        divisor /= 10;
        if (divisor == 1)
            started = 1;
    }
}
