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
