#ifndef NITRO_FX_MTX_H
#define NITRO_FX_MTX_H

#include <nitro/fx.h>

typedef struct MtxFx44 {
    fx32 _00, _01, _02, _03;
    fx32 _10, _11, _12, _13;
    fx32 _20, _21, _22, _23;
    fx32 _30, _31, _32, _33;
} MtxFx44;

typedef struct MtxFx43 {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
    fx32 _30, _31, _32;
} MtxFx43;

#ifdef __cplusplus
extern "C" {
#endif

void MTX_Concat44(const MtxFx44 *left, const MtxFx44 *right, MtxFx44 *result);
void MTX_ScaleApply44(const MtxFx44 *source, MtxFx44 *result, fx32 x, fx32 y, fx32 z);
void MTX_TransApply44(const MtxFx44 *source, MtxFx44 *result, fx32 x, fx32 y, fx32 z);

#ifdef __cplusplus
}
#endif

#endif
