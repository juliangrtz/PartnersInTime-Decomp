/*
 * Object link positions (overlay 2, 0x020BDB38-0x020BDC18).
 *
 * Sums link segments and averages their cumulative positions.
 */

#include <game/battle_object_link.h>
extern int _s32_div_f(int numerator, int denominator);
/* count is positive for an initialized flexible link. Each segment is a
 * displacement, so accumulate positions before averaging them. */
void BattleObjectLink_AveragePositions(s32 *out, const BattleObjectLinkSegment *segments, int count)
{
    int x = 0, y = 0, z = 0;
    int sum_x = 0, sum_y = 0, sum_z = 0;
    int remaining = count;
    for (; remaining; --remaining, ++segments) {
        x += segments->x;
        y += segments->y;
        z += segments->z;
        sum_x += x;
        sum_y += y;
        sum_z += z;
    }
    out[0] = _s32_div_f(sum_x, count);
    out[1] = _s32_div_f(sum_y, count);
    out[2] = _s32_div_f(sum_z, count);
}

void BattleObjectLink_SumSegments(s32 *out, const BattleObjectLinkSegment *segments, int count) {
    int x = 0, y = 0, z = 0;
    for (; count; --count, ++segments) {
        x += segments->x;
        y += segments->y;
        z += segments->z;
    }
    out[0] = x;
    out[1] = y;
    out[2] = z;
}
