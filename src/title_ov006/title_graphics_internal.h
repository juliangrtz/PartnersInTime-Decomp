#ifndef PIT_TITLE_GRAPHICS_INTERNAL_H
#define PIT_TITLE_GRAPHICS_INTERNAL_H
#include <nitro.h>
#define REG32(address) (*(volatile u32 *)(address))

static inline void PolygonAttributes(int lights, int mode, int cull, int polygon, int alpha, int flags)
{
    REG32(0x040004a4) = lights | (mode << 4) | (cull << 6) | (polygon << 24) | (alpha << 16) | flags;
}

static inline void TexCoord(int s, int t)
{
    REG32(0x04000488) = (u16)(s16)(s >> 8) | ((u16)(s16)(t >> 8) << 16);
}
static inline void Vertex(s16 x, s16 y, s16 z)
{
    REG32(0x0400048c) = (u16)x | ((u16)y << 16);
    REG32(0x0400048c) = (u16)z;
}
static inline void VertexXY(s16 x, s16 y)
{
    REG32(0x04000494) = (u16)x | ((u16)y << 16);
}

static inline void Translate(int x, int y, int depth)
{
    REG32(0x04000470) = x;
    REG32(0x04000470) = y;
    REG32(0x04000470) = depth;
}

static inline void Scale(int x, int y, int z)
{
    REG32(0x0400046c) = x;
    REG32(0x0400046c) = y;
    REG32(0x0400046c) = z;
}

#endif
