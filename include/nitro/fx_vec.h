#ifndef NITRO_FX_VEC_H
#define NITRO_FX_VEC_H

#include <nitro/fx.h>

typedef struct VecFx32 { fx32 x, y, z; } VecFx32;

#ifdef __cplusplus
extern "C" {
#endif

fx32 VEC_DotProduct(const VecFx32 *left, const VecFx32 *right);
void VEC_CrossProduct(const VecFx32 *left, const VecFx32 *right, VecFx32 *result);
void VEC_Normalize(const VecFx32 *source, VecFx32 *result);

#ifdef __cplusplus
}
#endif

#endif
