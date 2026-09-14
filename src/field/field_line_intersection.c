#include <game/field_geometry.h>
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
