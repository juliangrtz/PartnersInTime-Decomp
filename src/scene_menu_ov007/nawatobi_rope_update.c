#include "nawatobi_rope_internal.h"

void NawatobiRope_Update(NawatobiRope *rope, int iterations)
{
    int i;
    NawatobiRope_IntegratePoints(rope->points, 32);
    for (i = 0; i < iterations; ++i) {
        func_ov007_0208b1e8(rope->links, 31);
        NawatobiRope_ConstrainPoints(rope->points, 32);
    }
    NawatobiRope_CheckLinkCollisions(rope->links, 31);
    NawatobiRope_UpdateVelocities(rope->points, 32);
}
