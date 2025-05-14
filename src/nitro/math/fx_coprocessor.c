#include <hardware.h>
#include <nitro/fx.h>

// Shift division result by this much after fx32 division, to get fx32 result
#define FX_DIV_SHIFT (32 - FX32B_INT)

// The value 1 in shifted divion result units
#define FX_DIV_ONE (1 << (FX_DIV_SHIFT))

// The value 0.5 in shifted division result units
#define FX_DIV_HALF (FX_DIV_ONE >> 1)

// Start fixed-point division using math coprocessor
void FX_DivAsync(fx32 numer, fx32 denom) {
    *rDIVCNT = DIVCNT_MODE_64_32;

    // Shift numerator by 32 to avoid precision loss
    *rDIV_NUMER = (u64)numer << 32;
    *rDIV_DENOM = (u32)denom;
}

// Get result of fixed-point division from math coprocessor
fx32 FX_GetDivResult() {
    while (*rDIVCNT & DIVCNTF_BUSY) {}

    // Shift result back to account for precision loss prevention.
    // Also add a half, to prevent rouding errors.
    return (*rDIV_RESULT + FX_DIV_HALF) >> FX_DIV_SHIFT;
}

// Get result of fixed-point division from math coprocessor
fx64c FX_GetDivResultFx64c() {
    while (*rDIVCNT & DIVCNTF_BUSY) {}
    return *rDIV_RESULT;
}

// Fixed-point division using math coprocessor
fx32 FX_Div(fx32 numer, fx32 denom) {
    FX_DivAsync(numer, denom);
    return FX_GetDivResult();
}

// Signed integer modulo using math coprocessor
s32 FX_ModS32(s32 numer, s32 denom) {
    *rDIVCNT = DIVCNT_MODE_32_32;
    *rDIV_NUMER_L = (u32)numer;
    *rDIV_DENOM = (u32)denom;

    while (*rDIVCNT & DIVCNTF_BUSY) {}
    return *rDIVREM_RESULT_L;
}



// Shift sqrt result by this much after fx32 square root, to get fx32 result
#define FX_SQRT_SHIFT ((32 + FX32B_INT) / 2 - FX32B_INT)

// The value 1 in shifted square root result units
#define FX_SQRT_ONE (1 << (FX_SQRT_SHIFT))

// The value 0.5 in shifted square root result units
#define FX_SQRT_HALF (FX_SQRT_ONE >> 1)

// Start fixed-point square root using math coprocessor
void FX_SqrtAsync(fx32 x) {
    if (x > 0) {
        *rSQRTCNT = SQRTCNT_MODE_64;

        // Shift operand by 32 to avoid precision loss
        *rSQRT_PARAM = (u64)x << 32;
    }
    else {
        *rSQRTCNT = SQRTCNT_MODE_64;
        *rSQRT_PARAM = 0;
    }
}

// Get result of fixed-point square root from math coprocessor
fx32 FX_GetSqrtResult(void) {
    while (*rSQRTCNT & SQRTCNTF_BUSY) {}

    // Shift result back to account for precision loss prevention.
    // Also add a half, to prevent rouding errors.
    return (*rSQRT_RESULT + FX_SQRT_HALF) >> FX_SQRT_SHIFT;
}

// Fixed-point square root using math coprocessor
fx32 FX_Sqrt(fx32 x) {
    if (x <= 0) {
        return 0;
    }

    *rSQRTCNT = SQRTCNT_MODE_64;
    *rSQRT_PARAM = (u64)x << 32;
    return FX_GetSqrtResult();
}



// Start calculating 1.0 / x using math coprocessor
void FX_InvAsync(fx32 x) {
    *rDIVCNT = DIVCNT_MODE_64_32;
    *rDIV_NUMER = (u64)FX32_ONE << 32;
    *rDIV_DENOM = (u32)x;
}

// Calculate 1.0 / x using math coprocessor
fx32 FX_Inv(fx32 x) {
    FX_InvAsync(x);
    return FX_GetDivResult();
}

// Calculate the inverse square root of x using math coprocessor
fx32 FX_InvSqrt(fx32 x) {
    fx64c divResult;
    s64 sqrtResult;
    s64 invSqrtResult;

    if (x > 0) {
        FX_InvAsync(x);
        FX_SqrtAsync(x);

        divResult = FX_GetDivResultFx64c();
        while (*rSQRTCNT & SQRTCNTF_BUSY) {}
        sqrtResult = *(s32*)rSQRT_RESULT;

        // Shift result back to account for precision loss prevention.
        // Also add a half, to prevent rouding errors.
        invSqrtResult = divResult * sqrtResult;
        return (invSqrtResult + FX_SQRT_HALF) >> FX_SQRT_SHIFT;
    }
    else {
        return 0;
    }
}
