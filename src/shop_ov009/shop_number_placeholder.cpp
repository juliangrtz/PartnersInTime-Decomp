#include "shop_graphics_internal.h"
extern "C" {
#include <game/overlay005_display.h>
void MIi_CpuClear16(u16 value, void *destination, u32 size);
void func_0202ce08(const void *source, void *destination, int width, int height);
}

static inline void ClearScratch(int length)
{
    /* The native fill call reloads its value from a stack halfword. */
    volatile u16 clear = 0;
    MIi_CpuClear16(clear, SHOP_NUMBER_SCRATCH, length);
}

extern "C" void ShopNumber_DrawPlaceholder(int tile, u32 x)
{
    ClearScratch(512);
    u8 *glyph;
    int i = 0;
    glyph = SHOP_PACKED_GLYPHS + 2688;
    do {
        u8 *destination = SHOP_NUMBER_SCRATCH + (x >> 1) + 96;
        if (x & 1)
            func_ov009_0207caec(glyph, destination, 4, 8, 16);
        else
            func_ov009_0207cbf0(glyph, destination, 4, 8, 16);
        x = (u16)(x + 6);
        ++i;
    } while (i < 3);
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_SUB) + 32 * tile;
    func_0202ce08(SHOP_NUMBER_SCRATCH, destination, 16, 16);
}
