/*
 * Credits textured quad (overlay 6, 0x02079E88-0x0207A034).
 *
 * The quad helper the credits scene draws its illustrations and backdrop with.
 */

#include "title_graphics_internal.h"
#include "frontend_scene_internal.h"
void Credits_DrawTexturedQuad(int x, int y, int s, int t, int width, int height, int alpha,
                              int palette, int format, int scale) {
    if (!alpha)
        return;
    REG32(0x040004ac) = (u32)(palette * 32) >> (4 - (format == 2));
    PolygonAttributes(0, 0, 3, data_ov006_0207c594.polygon_id, alpha, 0);
    REG32(0x04000444) = 0;
    Translate(x, y, data_ov006_0207c594.draw_depth << 12);
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
