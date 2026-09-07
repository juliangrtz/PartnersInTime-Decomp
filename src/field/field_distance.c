#include <game/field_geometry.h>
#define FIELD_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))
/* The SDK exposes CLZ as one instruction; all scaling and arithmetic is C. */
static inline int LeadingZeros(u32 value)
{
    asm { clz value, value }
    return value;
}

fx32 FieldGeometry_GetOrbitLength(fx32 radius_x, fx32 radius_y)
{
    /* Preserve the game's asymmetric approximation: x*x + (y*y)/2.
     * Circular or zero-secondary-radius paths use pi times the diameter. */
    if (!radius_y || radius_x == radius_y)
        return (fx32)(((s64)(radius_x * 2) * 0x3243F6A89LL + 0x80000000LL) >> 32);
    return (fx32)(((s64)FX_Sqrt(FIELD_MUL(radius_x, radius_x) + FIELD_MUL(radius_y, radius_y) / 2) *
                       0x6487ED512LL +
                   0x80000000LL) >>
                  32);
}

fx32 FieldGeometry_GetVectorLength(fx32 x, fx32 y, fx32 z)
{
    int minimum = 32;
    int zeros_x, zeros_y, zeros_z;
    fx32 length, square_x, square_y, square_z;
    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;
    if (z < 0)
        z = -z;
    zeros_x = LeadingZeros(x);
    zeros_y = LeadingZeros(y);
    zeros_z = LeadingZeros(z);
    if (minimum > zeros_x)
        minimum = zeros_x;
    if (minimum > zeros_y)
        minimum = zeros_y;
    if (minimum > zeros_z)
        minimum = zeros_z;
    if (minimum <= 16) {
        y >>= 17 - minimum;
        x >>= 17 - minimum;
        z >>= 17 - minimum;
    }
    square_y = FIELD_MUL(y, y);
    square_x = FIELD_MUL(x, x);
    square_z = FIELD_MUL(z, z);
    length = FX_Sqrt(square_x + square_y + square_z);
    if (minimum <= 16)
        length <<= 17 - minimum;
    return length;
}
