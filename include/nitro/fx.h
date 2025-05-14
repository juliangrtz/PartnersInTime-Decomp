    #ifndef NITRO_MATH_FX_H
    #define NITRO_MATH_FX_H

#include <nitro.h>

typedef s32 fx32;

#define FX32B_INT           12
#define FX32F_INT           (0x3FFFF << FX32B_INT)
#define FX32B_DEC           0
#define FX32F_DEC           (0xFFF << FX32B_DEC)

#define FX32_MAX            ((fx32)0x7FFFFFFF)
#define FX32_MIN            ((fx32)0x80000000)

#define FX32_CONST(x)       ((fx32)(x * (1 << FX32B_INT)))
#define FX32_ONE            FX32_CONST(1.0f)



typedef s64 fx64;

#define FX64B_INT           12
#define FX64F_INT           (0x3FFFFFFFFFFFF << FX64B_INT)
#define FX64B_DEC           0
#define FX64F_DEC           (0xFFF << FX32B_DEC)

#define FX64_MAX            ((fx64)0x7fffffffffffffff)
#define FX64_MIN            ((fx64)0x8000000000000000)

#define FX64_CONST(x)       ((fx64)(x * (1 << FX64B_INT)))
#define FX64_ONE            FX64_CONST(1.0f)



typedef s64 fx64c;

#define FX64CB_INT           32
#define FX64CF_INT           (0xFFFFFFFF << FX64CB_INT)
#define FX64CB_DEC           0
#define FX64CF_DEC           (0xFFFFFFFF << FX32CB_DEC)

#define FX64C_MAX            ((fx64)0x7fffffffffffffff)
#define FX64C_MIN            ((fx64)0x8000000000000000)

#define FX64C_CONST(x)       ((fx64)(x * (1 << FX64CB_INT)))
#define FX64C_ONE            FX64C_CONST(1.0f)



void FX_DivAsync(fx32 numer, fx32 denom);
fx32 FX_GetDivResult();
fx64c FX_GetDivResultFx64c();
fx32 FX_Div(fx32 numer, fx32 denom);
s32 FX_DivS32(s32 numer, s32 denom);

void FX_SqrtAsync(fx32 x);
fx32 FX_GetSqrtResult();
fx32 FX_Sqrt(fx32 x);

void FX_InvAsync(fx32 x);
fx32 FX_Inv(fx32 x);
fx32 FX_InvSqrt(fx32 x);

    #endif
