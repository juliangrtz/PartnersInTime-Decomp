#ifndef PIT_TITLE_SPRITE_RENDER_INTERNAL_H
#define PIT_TITLE_SPRITE_RENDER_INTERNAL_H
#include <game/title_sprite_sequence.h>
#define REG32(address) (*(volatile u32 *)(address))
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
