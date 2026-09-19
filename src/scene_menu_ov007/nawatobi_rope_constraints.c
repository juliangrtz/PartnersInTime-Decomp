/*
 * Jump-rope rope constraints (overlay 7, 0x0208B0C8-0x0208B1E8).
 *
 * Damps the rope points' velocities and pulls them back to their rest
 * distances, the constraint half of the rope simulation.
 */

#include "nawatobi_rope_internal.h"

void NawatobiRope_UpdateVelocities(NawatobiRopePoint *points, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++points) {
        if (!points->pinned) {
            points->velocity_x = (230 * (points->x - points->previous_x)) >> 8;
            points->velocity_y = (230 * (points->y - points->previous_y)) >> 8;
            points->velocity_z = (230 * (points->z - points->previous_z)) >> 8;
            points->previous_x = points->x;
            points->previous_y = points->y;
            points->previous_z = points->z;
        }
        points->pinned = 0;
    }
}

void NawatobiRope_ConstrainPoints(NawatobiRopePoint *points, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++points) {
        if (points->z > 0)
            points->z = 0;
        if (points->pinned) {
            points->velocity_x = 0;
            points->velocity_y = 0;
            points->velocity_z = 0;
            points->x = points->previous_x;
            points->y = points->previous_y;
            points->z = points->previous_z;
        }
    }
}
