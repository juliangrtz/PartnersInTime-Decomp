#include "nawatobi_rope_internal.h"

void NawatobiRope_IntegratePoints(NawatobiRopePoint *points, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++points) {
        points->x += points->velocity_x;
        points->y += points->velocity_y;
        points->z += points->velocity_z;
        if (!points->pinned)
            points->z += 1638;
    }
}
