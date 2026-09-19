/*
 * Object link length (overlay 2, 0x020BDB38-0x020BDB88).
 *
 * Sums a link's segment lengths.
 */

#include <game/battle_object_link.h>
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
