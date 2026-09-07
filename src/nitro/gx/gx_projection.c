#include <hardware.h>
#include <nitro/gx_projection.h>

static inline void G3i_InvAsync(fx32 denominator)
{
    *rDIV_NUMER = (u64)FX32_ONE << 32;
    *rDIV_DENOM = (u32)denominator;
}

static inline fx32 G3i_MulFx32(fx32 left, fx32 right)
{
    return ((fx64)left * right + 0x800) >> 12;
}

void G3i_PerspectiveW(fx32 sine, fx32 cosine, fx32 aspect, fx32 near_plane, fx32 far_plane, fx32 scale_w, int load, MtxFx44 *result)
{
    volatile fx32 *fifo;
    fx32 cotangent;
    fx32 x;
    fx64c inverse;
    fx32 z;
    fx32 translation;
    cotangent = FX_Div(cosine, sine);
    if (scale_w != FX32_ONE) cotangent = cotangent * scale_w / FX32_ONE;
    *rDIV_NUMER = (u64)cotangent << 32;
    *rDIV_DENOM = (u32)aspect;
    if (load) {
        *(vu32 *)0x04000440 = 0;
        fifo = (volatile fx32 *)0x04000458;
    }
    if (result) {
        result->_01 = 0;
        result->_02 = 0;
        result->_03 = 0;
        result->_10 = 0;
        result->_12 = 0;
        result->_13 = 0;
        result->_20 = 0;
        result->_21 = 0;
        result->_23 = -scale_w;
        result->_30 = 0;
        result->_31 = 0;
        result->_33 = 0;
    }
    x = FX_GetDivResult();
    G3i_InvAsync(near_plane - far_plane);
    if (load) {
        *fifo = x;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = cotangent;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
    }
    if (result) {
        result->_00 = x;
        result->_11 = cotangent;
    }
    inverse = FX_GetDivResultFx64c();
    if (scale_w != FX32_ONE) inverse = inverse * scale_w / FX32_ONE;
    z = (inverse * (far_plane + near_plane) + ((fx64)1 << 31)) >> 32;
    translation = (inverse * G3i_MulFx32(2 * near_plane, far_plane) + ((fx64)1 << 31)) >> 32;
    if (load) {
        *fifo = z;
        *fifo = -scale_w;
        *fifo = 0;
        *fifo = 0;
        *fifo = translation;
        *fifo = 0;
    }
    if (result) {
        result->_22 = z;
        result->_32 = translation;
    }
}

void G3i_OrthoW(fx32 top, fx32 bottom, fx32 left, fx32 right, fx32 near_plane, fx32 far_plane, fx32 scale_w, int load, MtxFx44 *result)
{
    fx64c inverse_x, inverse_y, inverse_z;
    fx32 x, y, z;
    fx32 translation_x, translation_y, translation_z;
    volatile fx32 *fifo;
    FX_InvAsync(right - left);
    if (load) {
        *(vu32 *)0x04000440 = 0;
        fifo = (volatile fx32 *)0x04000458;
    }
    if (result) {
        result->_01 = 0;
        result->_02 = 0;
        result->_03 = 0;
        result->_10 = 0;
        result->_12 = 0;
        result->_13 = 0;
        result->_20 = 0;
        result->_21 = 0;
        result->_23 = 0;
        result->_33 = scale_w;
    }
    inverse_x = FX_GetDivResultFx64c();
    G3i_InvAsync(top - bottom);
    if (scale_w != FX32_ONE) inverse_x = inverse_x * scale_w / FX32_ONE;
    x = ((inverse_x << 13) + ((fx64)1 << 31)) >> 32;
    if (load) {
        *fifo = x;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
    }
    if (result) result->_00 = x;
    inverse_y = FX_GetDivResultFx64c();
    G3i_InvAsync(near_plane - far_plane);
    if (scale_w != FX32_ONE) inverse_y = inverse_y * scale_w / FX32_ONE;
    y = ((inverse_y << 13) + ((fx64)1 << 31)) >> 32;
    if (load) {
        *fifo = y;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
        *fifo = 0;
    }
    if (result) result->_11 = y;
    inverse_z = FX_GetDivResultFx64c();
    if (scale_w != FX32_ONE) inverse_z = inverse_z * scale_w / FX32_ONE;
    z = ((inverse_z << 13) + ((fx64)1 << 31)) >> 32;
    if (load) {
        *fifo = z;
        *fifo = 0;
    }
    if (result) result->_22 = z;
    translation_x = (inverse_x * -(right + left) + ((fx64)1 << 31)) >> 32;
    translation_y = (inverse_y * -(top + bottom) + ((fx64)1 << 31)) >> 32;
    translation_z = (inverse_z * (far_plane + near_plane) + ((fx64)1 << 31)) >> 32;
    if (load) {
        *fifo = translation_x;
        *fifo = translation_y;
        *fifo = translation_z;
        *fifo = scale_w;
    }
    if (result) {
        result->_30 = translation_x;
        result->_31 = translation_y;
        result->_32 = translation_z;
    }
}

void G3i_LookAt(const VecFx32 *position, const VecFx32 *up, const VecFx32 *target, int load, MtxFx43 *result)
{
    VecFx32 forward, side, vertical;
    fx32 x, y, z;
    volatile fx32 *fifo;
    forward.x = position->x - target->x;
    forward.y = position->y - target->y;
    forward.z = position->z - target->z;
    VEC_Normalize(&forward, &forward);
    VEC_CrossProduct(up, &forward, &side);
    VEC_Normalize(&side, &side);
    VEC_CrossProduct(&forward, &side, &vertical);
    if (load) {
        *(vu32 *)0x04000440 = 2;
        fifo = (volatile fx32 *)0x0400045c;
        *fifo = side.x;
        *fifo = vertical.x;
        *fifo = forward.x;
        *fifo = side.y;
        *fifo = vertical.y;
        *fifo = forward.y;
        *fifo = side.z;
        *fifo = vertical.z;
        *fifo = forward.z;
    }
    x = -VEC_DotProduct(position, &side);
    y = -VEC_DotProduct(position, &vertical);
    z = -VEC_DotProduct(position, &forward);
    if (load) {
        *fifo = x;
        *fifo = y;
        *fifo = z;
    }
    if (result) {
        result->_00 = side.x;
        result->_01 = vertical.x;
        result->_02 = forward.x;
        result->_10 = side.y;
        result->_11 = vertical.y;
        result->_12 = forward.y;
        result->_20 = side.z;
        result->_21 = vertical.z;
        result->_22 = forward.z;
        result->_30 = x;
        result->_31 = y;
        result->_32 = z;
    }
}
