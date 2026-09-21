/*
 * HUD decimal sprites (overlay 0, 0x02071DE8-0x02071EBC).
 * Emit digits from right to left, stopping when the quotient reaches zero.
 * The fourth OAM halfword belongs to affine matrices and remains untouched.
 */
#include <game/field_hud.h>

extern "C" GameOamEntry *FieldHud_DrawNumber(void *context, GameOamEntry *output,
                                        s16 x, int y, int value,
                                        int digits, u32 graphics)
{
    u16 tile = graphics >> 7;
    x += 7 * (digits - 1);
    for (int i = 0; i < digits; ++i, x -= 7) {
        *(u32 *)output = (y & 255) | 0xA000 | ((x & 511) << 16);
        output->attributes[2] = (tile + (u16)(value % 10)) | 0xF000;
        ++output;
        value /= 10;
        if (value == 0)
            break;
    }
    return output;
}
