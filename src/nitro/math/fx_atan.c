#include <nitro/fx_atan.h>

extern const s16 FX_AtanTable_[];
extern const s16 FX_AtanIdxTable_[];

int FX_AtanIdx(fx32 tangent)
{
    if (tangent >= 0) {
        if (tangent > FX32_ONE) return (u16)(0x4000 - FX_AtanIdxTable_[FX_Inv(tangent) >> 5]);
        if (tangent < FX32_ONE) return (u16)FX_AtanIdxTable_[tangent >> 5];
        return 0x2000;
    } else {
        if (tangent < -FX32_ONE) return (u16)(FX_AtanIdxTable_[FX_Inv(-tangent) >> 5] - 0x4000);
        if (tangent > -FX32_ONE) return (u16)-FX_AtanIdxTable_[-tangent >> 5];
        return 0xe000;
    }
}

int FX_Atan2Idx(fx32 y, fx32 x)
{
    fx32 numerator;
    int angle;
    int add;
    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                numerator = y;
                angle = 0;
                add = 1;
            } else if (x < y) {
                numerator = x;
                x = y;
                angle = 0x4000;
                add = 0;
            } else return 0x2000;
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                numerator = x;
                x = y;
                angle = 0x4000;
                add = 1;
            } else if (x > y) {
                numerator = y;
                angle = 0x8000;
                add = 0;
            } else return 0x6000;
        } else return 0x4000;
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                numerator = y;
                angle = -0x8000;
                add = 1;
            } else if (x < y) {
                numerator = x;
                x = y;
                angle = -0x4000;
                add = 0;
            } else return 0xa000;
        } else if (x > 0) {
            if (x < y) {
                numerator = x;
                x = y;
                angle = -0x4000;
                add = 1;
            } else if (x > y) {
                numerator = y;
                angle = 0;
                add = 0;
            } else return 0xe000;
        } else return 0xc000;
    } else {
        if (x >= 0) return 0;
        return 0x8000;
    }
    if (!x) return 0;
    if (add) return (u16)(angle + FX_AtanIdxTable_[FX_Div(numerator, x) >> 5]);
    return (u16)(angle - FX_AtanIdxTable_[FX_Div(numerator, x) >> 5]);
}

int FX_Atan2(fx32 y, fx32 x)
{
    fx32 numerator;
    int angle;
    int add;
    if (y > 0) {
        if (x > 0) {
            if (x > y) {
                numerator = y;
                angle = 0;
                add = 1;
            } else if (x < y) {
                numerator = x;
                x = y;
                angle = 6434;
                add = 0;
            } else return 3217;
        } else if (x < 0) {
            x = -x;
            if (x < y) {
                numerator = x;
                x = y;
                angle = 6434;
                add = 1;
            } else if (x > y) {
                numerator = y;
                angle = 12868;
                add = 0;
            } else return 9651;
        } else return 6434;
    } else if (y < 0) {
        y = -y;
        if (x < 0) {
            x = -x;
            if (x > y) {
                numerator = y;
                angle = -12868;
                add = 1;
            } else if (x < y) {
                numerator = x;
                x = y;
                angle = -6434;
                add = 0;
            } else return -9651;
        } else if (x > 0) {
            if (x < y) {
                numerator = x;
                x = y;
                angle = -6434;
                add = 1;
            } else if (x > y) {
                numerator = y;
                angle = 0;
                add = 0;
            } else return -3217;
        } else return -6434;
    } else {
        if (x >= 0) return 0;
        return 12868;
    }
    if (!x) return 0;
    if (add) return (s16)(angle + FX_AtanTable_[FX_Div(numerator, x) >> 5]);
    return (s16)(angle - FX_AtanTable_[FX_Div(numerator, x) >> 5]);
}

