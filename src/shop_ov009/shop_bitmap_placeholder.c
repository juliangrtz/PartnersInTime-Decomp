#include "shop_graphics_internal.h"

void ShopBitmap_DrawPlaceholder(int x, int y)
{
    const u8 *glyph = SHOP_BITMAP_GLYPHS + 5376;
    int digit;
    func_ov009_0207c558(x, y, 20, 12);
    for (digit = 0; digit < 3; ++digit, x += 6)
        func_ov009_0207c464(x, y + 2, glyph, 8, 8, 48, 1);
}
