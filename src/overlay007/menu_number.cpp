#include "pause_scene_internal.h"
extern "C" {
#include <game/menu_number.h>
const u8 *func_ov007_02077718(const u8 *, u8 *, int, int, int);
const u8 *func_ov007_0207781c(const u8 *, u8 *, int, int, int);
void MIi_CpuClear16(u16, void *, u32);

void func_0202ce08(const void *, void *, int, int);
}
static inline void ClearGlyphScratch(void)
{
    volatile u16 zero = 0;
    MIi_CpuClear16(zero, WORK.number_scratch, 512);
}
extern "C" void MenuNumber_DrawValue(int tile, u32 x, int value, int wide)
{
    ClearGlyphScratch();
    int visible = 0, divisor = 100;
    int width, advance, glyph_size;
    u8 *glyphs;
    if (!wide) {
        width = 4;
        glyphs = (u8 *)WORK.number_glyph_pixels;
        advance = 7;
        glyph_size = 64;
    } else {
        width = 8;
        glyphs = (u8 *)WORK.number_glyph_pixels + 640;
        advance = 9;
        glyph_size = 128;
    }
    int i = 0;
    do {
        int digit = value / divisor;
        if (digit || visible) {
            visible = 1;
            u8 *destination = (u8 *)WORK.number_scratch + (x >> 1);
            if (x & 1)
                func_ov007_02077718(glyphs + glyph_size * digit, destination, width, 16, 16);
            else
                func_ov007_0207781c(glyphs + glyph_size * digit, destination, width, 16, 16);
        }
        value = (u16)(value % divisor);
        divisor /= 10;
        x = (u16)(x + advance);
        if (divisor == 1)
            visible = 1;
        ++i;
    } while (i < 3);
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + 32 * tile;
    func_0202ce08(WORK.number_scratch, destination, 16, 16);
}
extern "C" void MenuNumber_DrawUnavailable(int tile, u32 x)
{
    ClearGlyphScratch();
    u8 *glyph;
    int i = 0;
    glyph = (u8 *)WORK.number_glyph_pixels + 1984;
    do {
        u8 *destination = (u8 *)WORK.number_scratch + (x >> 1) + 128;
        if (x & 1)
            func_ov007_02077718(glyph, destination, 8, 8, 16);
        else
            func_ov007_0207781c(glyph, destination, 8, 8, 16);
        x = (u16)(x + 7);
        ++i;
    } while (i < 3);
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + 32 * tile;
    func_0202ce08(WORK.number_scratch, destination, 16, 16);
}
