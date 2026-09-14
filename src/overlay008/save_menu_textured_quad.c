#include "../overlay006/title_graphics_internal.h"
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
