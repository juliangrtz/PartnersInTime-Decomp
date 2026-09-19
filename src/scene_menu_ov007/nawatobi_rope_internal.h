#ifndef PIT_NAWATOBI_ROPE_INTERNAL_H
#define PIT_NAWATOBI_ROPE_INTERNAL_H
#include <nitro.h>

typedef struct NawatobiRopePoint {
    s32 x, y, z;
    s32 previous_x, previous_y, previous_z;
    s32 velocity_x, velocity_y, velocity_z;
    u16 pinned, unknown38;
} NawatobiRopePoint;
typedef struct NawatobiRopeLink {
    NawatobiRopePoint *first, *second;
    s32 length;
} NawatobiRopeLink;
typedef struct NawatobiRope {
    NawatobiRopePoint points[32];
    NawatobiRopeLink links[31];
} NawatobiRope;

#ifdef __cplusplus
extern "C" {
#endif
void func_ov007_0208aa78(NawatobiRopePoint *, NawatobiRopePoint *);
void func_ov007_0208b1e8(NawatobiRopeLink *, int);
void NawatobiRope_IntegratePoints(NawatobiRopePoint *points, int count);
void NawatobiRope_UpdateVelocities(NawatobiRopePoint *points, int count);
void NawatobiRope_ConstrainPoints(NawatobiRopePoint *points, int count);
void NawatobiRope_Update(NawatobiRope *rope, int iterations);
void NawatobiRope_CheckLinkCollisions(NawatobiRopeLink *links, int count);
#ifdef __cplusplus
}
#endif
typedef char NawatobiRopePointSize[sizeof(NawatobiRopePoint) == 40 ? 1 : -1];
typedef char NawatobiRopeLinkSize[sizeof(NawatobiRopeLink) == 12 ? 1 : -1];
typedef char NawatobiRopeSize[sizeof(NawatobiRope) == 1652 ? 1 : -1];
#endif
