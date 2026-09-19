/*
 * Jump-rope collisions (overlay 7, 0x0208AA38-0x0208AA78).
 *
 * Tests the rope's links against the jumper.
 */

#include "nawatobi_rope_internal.h"

void NawatobiRope_CheckLinkCollisions(NawatobiRopeLink *links, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++links)
        func_ov007_0208aa78(links->first, links->second);
}
