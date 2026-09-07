#ifndef NITRO_FX_ATAN_H
#define NITRO_FX_ATAN_H

#include <nitro/fx.h>

#ifdef __cplusplus
extern "C" {
#endif

int FX_Atan2(fx32 y, fx32 x);
int FX_Atan2Idx(fx32 y, fx32 x);
int FX_AtanIdx(fx32 tangent);

#ifdef __cplusplus
}
#endif

#endif
