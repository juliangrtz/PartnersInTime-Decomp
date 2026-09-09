#include "pause_scene_internal.h"
extern "C" {
#include <game/menu_number.h>
void func_0202cf04(const void *, void *, int, int);
}
/* Convert the two ten-digit fonts and the final symbols from tiles to rows. */
extern "C" void MenuNumber_PrepareGlyphs(void)
{
    {
        u8 *source, *destination;
        int i = 0;
        source = (u8 *)WORK.number_glyph_tiles;
        destination = (u8 *)WORK.number_glyph_pixels;
        do {
            func_0202cf04(source, destination, 4, 16);
            ++i;
            source += 64;
            destination += 64;
        } while (i < 10);
    }
    {
        int i = 0;
        u8 *source = (u8 *)WORK.number_glyph_tiles + 640, *destination = (u8 *)WORK.number_glyph_pixels + 640;
        do {
            func_0202cf04(source, destination, 8, 16);
            ++i;
            source += 128;
            destination += 128;
        } while (i < 10);
    }
    func_0202cf04((u8 *)WORK.number_glyph_tiles + 1920, (u8 *)WORK.number_glyph_pixels + 1920, 8, 8);
    func_0202cf04((u8 *)WORK.number_glyph_tiles + 1984, (u8 *)WORK.number_glyph_pixels + 1984, 8, 8);
}
