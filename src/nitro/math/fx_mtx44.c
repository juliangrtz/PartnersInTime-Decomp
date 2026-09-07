#include <nitro/fx_mtx.h>

extern void MI_Copy48B(const void *source, void *destination);

void MTX_TransApply44(const MtxFx44 *source, MtxFx44 *result, fx32 x, fx32 y, fx32 z)
{
    if (source != result) MI_Copy48B(source, result);
    result->_30 = source->_30 + (fx32)(((fx64)x * source->_00 + (fx64)y * source->_10 + (fx64)z * source->_20) >> 12);
    result->_31 = source->_31 + (fx32)(((fx64)x * source->_01 + (fx64)y * source->_11 + (fx64)z * source->_21) >> 12);
    result->_32 = source->_32 + (fx32)(((fx64)x * source->_02 + (fx64)y * source->_12 + (fx64)z * source->_22) >> 12);
    result->_33 = source->_33 + (fx32)(((fx64)x * source->_03 + (fx64)y * source->_13 + (fx64)z * source->_23) >> 12);
}

void MTX_ScaleApply44(const MtxFx44 *source, MtxFx44 *result, fx32 x, fx32 y, fx32 z)
{
    result->_00 = ((fx64)x * source->_00) >> 12;
    result->_01 = ((fx64)x * source->_01) >> 12;
    result->_02 = ((fx64)x * source->_02) >> 12;
    result->_03 = ((fx64)x * source->_03) >> 12;
    result->_10 = ((fx64)y * source->_10) >> 12;
    result->_11 = ((fx64)y * source->_11) >> 12;
    result->_12 = ((fx64)y * source->_12) >> 12;
    result->_13 = ((fx64)y * source->_13) >> 12;
    result->_20 = ((fx64)z * source->_20) >> 12;
    result->_21 = ((fx64)z * source->_21) >> 12;
    result->_22 = ((fx64)z * source->_22) >> 12;
    result->_23 = ((fx64)z * source->_23) >> 12;
    if (source != result) {
        fx32 copy_x = source->_30;
        fx32 copy_y = source->_31;
        result->_30 = copy_x;
        result->_31 = copy_y;
        copy_x = source->_32;
        copy_y = source->_33;
        result->_32 = copy_x;
        result->_33 = copy_y;
    }
}

void MTX_Concat44(const MtxFx44 *left, const MtxFx44 *right, MtxFx44 *result)
{
    MtxFx44 temporary;
    MtxFx44 *output = result;
    fx32 x, y, z, w;
    fx32 b0, b1, b2, b3;
    if (result == right) output = &temporary;
    x = left->_00;
    y = left->_01;
    z = left->_02;
    w = left->_03;
    output->_00 = ((fx64)x * right->_00 + (fx64)y * right->_10 + (fx64)z * right->_20 + (fx64)w * right->_30) >> 12;
    output->_01 = ((fx64)x * right->_01 + (fx64)y * right->_11 + (fx64)z * right->_21 + (fx64)w * right->_31) >> 12;
    output->_03 = ((fx64)x * right->_03 + (fx64)y * right->_13 + (fx64)z * right->_23 + (fx64)w * right->_33) >> 12;
    b0 = right->_02;
    b1 = right->_12;
    b2 = right->_22;
    b3 = right->_32;
    output->_02 = ((fx64)x * b0 + (fx64)y * b1 + (fx64)z * b2 + (fx64)w * b3) >> 12;
    x = left->_10;
    y = left->_11;
    z = left->_12;
    w = left->_13;
    output->_12 = ((fx64)x * b0 + (fx64)y * b1 + (fx64)z * b2 + (fx64)w * b3) >> 12;
    output->_11 = ((fx64)x * right->_01 + (fx64)y * right->_11 + (fx64)z * right->_21 + (fx64)w * right->_31) >> 12;
    output->_13 = ((fx64)x * right->_03 + (fx64)y * right->_13 + (fx64)z * right->_23 + (fx64)w * right->_33) >> 12;
    b0 = right->_00;
    b1 = right->_10;
    b2 = right->_20;
    b3 = right->_30;
    output->_10 = ((fx64)x * b0 + (fx64)y * b1 + (fx64)z * b2 + (fx64)w * b3) >> 12;
    x = left->_20;
    y = left->_21;
    z = left->_22;
    w = left->_23;
    output->_20 = ((fx64)x * b0 + (fx64)y * b1 + (fx64)z * b2 + (fx64)w * b3) >> 12;
    output->_21 = ((fx64)x * right->_01 + (fx64)y * right->_11 + (fx64)z * right->_21 + (fx64)w * right->_31) >> 12;
    output->_23 = ((fx64)x * right->_03 + (fx64)y * right->_13 + (fx64)z * right->_23 + (fx64)w * right->_33) >> 12;
    b0 = right->_02;
    b1 = right->_12;
    b2 = right->_22;
    b3 = right->_32;
    output->_22 = ((fx64)x * b0 + (fx64)y * b1 + (fx64)z * b2 + (fx64)w * b3) >> 12;
    x = left->_30;
    y = left->_31;
    z = left->_32;
    w = left->_33;
    output->_32 = ((fx64)x * b0 + (fx64)y * b1 + (fx64)z * b2 + (fx64)w * b3) >> 12;
    output->_31 = ((fx64)x * right->_01 + (fx64)y * right->_11 + (fx64)z * right->_21 + (fx64)w * right->_31) >> 12;
    output->_30 = ((fx64)x * right->_00 + (fx64)y * right->_10 + (fx64)z * right->_20 + (fx64)w * right->_30) >> 12;
    output->_33 = ((fx64)x * right->_03 + (fx64)y * right->_13 + (fx64)z * right->_23 + (fx64)w * right->_33) >> 12;
    if (output == &temporary) *result = temporary;
}
