#include <hardware.h>
#include <nitro/fx_vec.h>

fx32 VEC_DotProduct(const VecFx32 *left, const VecFx32 *right)
{
    return ((fx64)left->x * right->x + (fx64)left->y * right->y +
        (fx64)left->z * right->z + 0x800) >> 12;
}

void VEC_CrossProduct(const VecFx32 *left, const VecFx32 *right, VecFx32 *result)
{
    fx32 x = ((fx64)left->y * right->z - (fx64)left->z * right->y + 0x800) >> 12;
    fx32 y = ((fx64)left->z * right->x - (fx64)left->x * right->z + 0x800) >> 12;
    fx32 z = ((fx64)left->x * right->y - (fx64)left->y * right->x + 0x800) >> 12;
    result->x = x;
    result->y = y;
    result->z = z;
}

void VEC_Normalize(const VecFx32 *source, VecFx32 *result)
{
    fx64 length_squared;
    fx32 root;
    fx64 reciprocal;
    fx64 scale;
    fx32 x = source->x;
    fx32 y = source->y;
    fx32 z = source->z;
    length_squared = (fx64)x * x + (fx64)y * y + (fx64)z * z;
    *rDIVCNT = DIVCNT_MODE_64_64;
    *rDIV_NUMER = (u64)1 << 56;
    *rDIV_DENOM = length_squared;
    *rSQRTCNT = SQRTCNT_MODE_64;
    *rSQRT_PARAM = length_squared << 2;
    while (*rSQRTCNT & SQRTCNTF_BUSY) {}
    root = *(vs32 *)rSQRT_RESULT;
    while (*rDIVCNT & DIVCNTF_BUSY) {}
    reciprocal = *(vs64 *)rDIV_RESULT;
    scale = reciprocal * root;
    result->x = (scale * source->x + ((fx64)1 << 44)) >> 45;
    result->y = (scale * source->y + ((fx64)1 << 44)) >> 45;
    result->z = (scale * source->z + ((fx64)1 << 44)) >> 45;
}
