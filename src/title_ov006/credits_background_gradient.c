#include "credits_transition_internal.h"

/* Unit quad strip, expanded to the screen size by the matrix below. */
static inline void DrawGradientQuad(u32 first, u32 second)
{
    REG32(0x04000480) = first;
    REG32(0x0400048c) = 0;
    REG32(0x0400048c) = 0;
    REG32(0x04000480) = second;
    REG32(0x0400048c) = 0x10000000;
    REG32(0x0400048c) = 0;
    REG32(0x04000480) = first;
    REG32(0x0400048c) = 0x1000;
    REG32(0x0400048c) = 0;
    REG32(0x04000480) = second;
    REG32(0x0400048c) = 0x10001000;
    REG32(0x0400048c) = 0;
}

void CreditsBackground_DrawGradient(void)
{
    int depth;

    REG32(0x040004a4) = (data_ov006_0207c594.polygon_id << 24) | 0x1f00c0;
    REG32(0x04000444) = 0;
    depth = data_ov006_0207c594.draw_depth << 12;
    REG32(0x04000470) = 0;
    REG32(0x04000470) = 0;
    REG32(0x04000470) = depth;
    REG32(0x0400046c) = 0x100000;
    REG32(0x0400046c) = 0xc0000;
    REG32(0x0400046c) = 0x1000;
    REG32(0x04000500) = 3;
    if (data_ov006_0207c594.background_variant) {
        DrawGradientQuad(0x3461, 0x7d41);
    } else {
        DrawGradientQuad(0x7d41, 0x7ea9);
    }
    REG32(0x04000504) = 0;
    REG32(0x04000448) = 1;
    ++data_ov006_0207c594.draw_depth;
    ++data_ov006_0207c594.polygon_id;
    func_ov006_020798c8();
}
