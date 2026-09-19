#include <game/field_geometry.h>
#define FIELD_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))
/* Reduce large components before squaring, then restore the length scale. */
void FieldGeometry_ScaleVector(fx32 x, fx32 y, fx32 z, fx32 length, fx32 *out_x, fx32 *out_y, fx32 *out_z)
{
    int shift = 0;
    fx32 scale;
    if (x >= 16777216 || x <= -16777216 || y >= 16777216 || y <= -16777216 || z >= 16777216 || z <= -16777216)
        shift = 8;
    else if (x >= 1048576 || x <= -1048576 || y >= 1048576 || y <= -1048576 || z >= 1048576 || z <= -1048576)
        shift = 4;
    scale = FX_Div(FX_Sqrt(FIELD_MUL(x >> shift, x >> shift) + FIELD_MUL(y >> shift, y >> shift) +
                           FIELD_MUL(z >> shift, z >> shift))
                       << shift,
                   length);
    if (scale > 0) {
        *out_x = FX_Div(x, scale);
        *out_y = FX_Div(y, scale);
        *out_z = FX_Div(z, scale);
    } else {
        *out_x = x;
        *out_y = y;
        *out_z = z;
    }
}

/* Keep deltas in Q12 words; widen only each rounded product. */
static inline fx32 RoundedMul(fx32 a, fx32 b) { return (fx32)(((s64)a * b + 2048) >> 12); }
void FieldGeometry_IntersectLines(fx32 x1, fx32 x2, fx32 y1, fx32 y2, fx32 x3, fx32 x4, fx32 y3,
                                  fx32 y4, fx32 *x, fx32 *y) {
    fx32 dx1 = x2 - x1, dy1 = y2 - y1, dx2 = x4 - x3, dy2 = y4 - y3;
    fx32 denominator = RoundedMul(dx1, dy2) - RoundedMul(dy1, dx2);
    if (denominator) {
        fx32 numerator = RoundedMul(y1 - y3, dx2) - RoundedMul(x1 - x3, dy2);
        *x = x1 + FX_Div(RoundedMul(dx1, numerator), denominator);
        *y = y1 + FX_Div(RoundedMul(dy1, numerator), denominator);
    } else {
        *x = x1;
        *y = y1;
    }
}

int FieldGeometry_GetOrientation(int x0, int y0, int x1, int y1, int x2, int y2)
{
    /* Round each Q12 product before taking the determinant's sign. */
    int cross = (int)(((s64)(x1 - x0) * (y2 - y1) + 2048) >> 12) -
                (int)(((s64)(y1 - y0) * (x2 - x1) + 2048) >> 12);
    if (cross > 0) return 1;
    return -(cross < 0);
}

const void *FieldResource_GetSection(const u32 *resource, int section)
{
    const void *result = 0;
    if (resource && FieldResource_GetSectionSize(resource, section))
        result = (const u8 *)resource + resource[section];
    return result;
}

u32 FieldResource_GetSectionSize(const u32 *resource, int section)
{
    u32 size = 0;
    if (resource) size = (resource + section)[1] - resource[section];
    return size;
}
