#include <game/shop_graphics.h>

static inline u8 LowNibble(int value) { return value & 15; }
static inline u8 HighNibble(int value) { return (value & 240) >> 4; }

extern "C" const u8 *ShopGlyph_Expand4bpp(const u8 *source, u8 *destination, int count)
{
    while (count--) {
        /* Keep both reads: a destination store can alias the source byte. */
        *destination++ = LowNibble(*source);
        *destination++ = HighNibble(*source);
        ++source;
    }
    return source;
}
