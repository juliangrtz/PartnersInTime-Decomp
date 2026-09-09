#ifndef PIT_GAME_FIELD_GEOMETRY_H
#define PIT_GAME_FIELD_GEOMETRY_H

#include <nitro/fx.h>

#ifdef __cplusplus
extern "C" {
#endif
void FieldGeometry_ScaleVector(fx32 x, fx32 y, fx32 z, fx32 length, fx32 *out_x, fx32 *out_y, fx32 *out_z);
fx32 FieldGeometry_GetVectorLength(fx32 x, fx32 y, fx32 z);
fx32 FieldGeometry_GetOrbitLength(fx32 radius_x, fx32 radius_y);
u32 FieldResource_GetSectionSize(const u32 *resource, int section);
const void *FieldResource_GetSection(const u32 *resource, int section);
int FieldGeometry_GetOrientation(fx32 x0, fx32 y0, fx32 x1, fx32 y1, fx32 x2, fx32 y2);
fx32 FieldGeometry_GetDirectionVector(int direction, fx32 length, fx32 *x, fx32 *y);
#ifdef __cplusplus
}
#endif

#endif
