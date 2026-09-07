#ifndef NITRO_GX_PROJECTION_H
#define NITRO_GX_PROJECTION_H

#include <nitro/fx_mtx.h>
#include <nitro/fx_vec.h>

#ifdef __cplusplus
extern "C" {
#endif

void G3i_LookAt(const VecFx32 *position, const VecFx32 *up, const VecFx32 *target, int load, MtxFx43 *result);
void G3i_OrthoW(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 near_plane, fx32 far_plane, fx32 scale_w, int load, MtxFx44 *result);
void G3i_PerspectiveW(fx32 sine, fx32 cosine, fx32 aspect, fx32 near_plane, fx32 far_plane, fx32 scale_w, int load, MtxFx44 *result);

#ifdef __cplusplus
}
#endif

#endif
