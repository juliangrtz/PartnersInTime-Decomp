/*
 * Save menu numbers (overlay 8, 0x02072F08-0x02073410).
 *
 * Draws the coin count and play time as textured quads, digit by digit.
 */

#include <game/save_menu.h>
#include "../title_ov006/title_graphics_internal.h"
/* Draw counters within the shared menu workspace. */
typedef struct MenuQuadWork {
    u8 prefix[0x188];
    int depth, polygon;
} MenuQuadWork;
typedef char MenuQuadWork_SizeCheck[sizeof(MenuQuadWork) == 0x190 ? 1 : -1];
extern u8 data_ov008_02078290[];
#define DRAW (*(MenuQuadWork *)data_ov008_02078290)
void SaveMenu_DrawTexturedQuad(int x, int y, int s, int t, int width, int height, int alpha,
                               int palette, int format, int scale) {
    if (!alpha)
        return;
    REG32(0x040004ac) = (u32)(palette * 32) >> (4 - (format == 2));
    PolygonAttributes(0, 0, 3, DRAW.polygon, alpha, 0);
    REG32(0x04000444) = 0;
    Translate(x, y, DRAW.depth << 12);
    Scale(width * scale, height * scale, 4096);
    REG32(0x04000480) = 32767;
    REG32(0x04000500) = 3;
    TexCoord(s, t);
    Vertex(0, 0, 0);
    TexCoord(s, t + (height << 12));
    Vertex(0, 4096, 0);
    TexCoord(s + (width << 12), t);
    Vertex(4096, 0, 0);
    TexCoord(s + (width << 12), t + (height << 12));
    Vertex(4096, 4096, 0);
    REG32(0x04000504) = 0;
    REG32(0x04000448) = 1;
}

void SaveMenu_DrawNumber(int x, int y, int value, int digits, int style, int leading, int alpha,
                         int scale) {
    int divisor = 1;
    int i;
    int s, t, advance, palette;
    REG32(0x040004a8) = 1833964544;
    for (i = 0; i < digits - 1; ++i)
        divisor *= 10;
    if (value >= 10 * divisor)
        value = 10 * divisor - 1;
    switch (style) {
    case 0:
        advance = 8 * scale;
        s = 0;
        t = 2048;
        palette = 3;
        break;
    case 1:
        advance = 8 * scale;
        s = 0;
        t = 67584;
        palette = 4;
        break;
    }
    for (i = 0; i < digits; ++i) {
        int digit = value / divisor;
        if (digit || leading) {
            leading = 1;
            SaveMenu_DrawTexturedQuad(x, y, s + (digit << 16), t, 16, 15, alpha, palette, 3, scale);
            ++DRAW.depth;
        }
        value %= divisor;
        divisor /= 10;
        if (divisor == 1)
            leading = 1;
        x += advance;
    }
}

extern u8 data_ov008_02078324[], data_ov008_02078328[];
void SaveMenu_DrawCoinsAndTime(int slot) {
    int offset = sizeof(SaveMenuSummary) * slot;
    int coins = *(s32 *)(data_ov008_02078324 + offset);
    u32 *time;
    int hours, minutes;
    u32 ticks;
    if (coins > 999999)
        coins = 999999;
    SaveMenu_DrawNumber(253952, 536576, coins, 6, 0, 0, 31, 4096);
    time = (u32 *)(data_ov008_02078328 + offset);
    hours = *time / 216000;
    if (hours > 99)
        hours = 99;
    SaveMenu_DrawNumber(299008, 610304, hours, 2, 1, 1, 31, 4096);
    SaveMenu_DrawTexturedQuad(360448, 610304, 655360, 67584, 8, 15, 31, 4, 3, 4096);
    ticks = *time;
    if (ticks >= 21596400)
        minutes = 59;
    else {
        minutes = ticks % 216000 / 3600;
        if (minutes > 59)
            minutes = 59;
    }
    SaveMenu_DrawNumber(385024, 610304, minutes, 2, 1, 1, 31, 4096);
}
