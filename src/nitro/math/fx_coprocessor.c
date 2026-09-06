#include <hardware.h>
#include <nitro/fx.h>

#define FX_DIV_SHIFT (32 - FX32B_INT)
#define FX_DIV_ONE (1 << FX_DIV_SHIFT)
#define FX_DIV_HALF (FX_DIV_ONE >> 1)
#define FX_SQRT_SHIFT ((32 + FX32B_INT) / 2 - FX32B_INT)
#define FX_SQRT_ONE (1 << FX_SQRT_SHIFT)
#define FX_SQRT_HALF (FX_SQRT_ONE >> 1)

/* MWCC emits these functions in reverse definition order. */
fx32 FX_Div(fx32 numer, fx32 denom) {
    FX_DivAsync(numer, denom);
    return FX_GetDivResult();
}

fx32 FX_Inv(fx32 x) {
    FX_InvAsync(x);
    return FX_GetDivResult();
}

fx32 FX_Sqrt(fx32 x) {
    if (x <= 0) {
        return 0;
    }

    /* Start the 64-bit hardware operation with the original MMIO schedule. */
    asm {
        ldr r2, =0x040002B0
        mov r3, #1
        strh r3, [r2, #0]
        ldr r1, =0x040002B8
        mov r2, #0
        str r2, [r1, #0]
        str x, [r1, #4]
    }
    return FX_GetSqrtResult();
}

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

        // The Q32 reciprocal times the Q22 root is Q54. Round and shift
        // by 42 bits to return Q12, retaining the full 64-bit product.
        invSqrtResult = divResult * sqrtResult;
        return (invSqrtResult + ((s64)FX_SQRT_HALF << 32)) >>
            (FX_SQRT_SHIFT + 32);
    }
    else {
        return 0;
    }
}

fx64c FX_GetDivResultFx64c() {
    while (*rDIVCNT & DIVCNTF_BUSY) {}
    return *rDIV_RESULT;
}

fx32 FX_GetDivResult() {
    while (*rDIVCNT & DIVCNTF_BUSY) {}

    // Shift result back to account for precision loss prevention.
    // Also add a half, to prevent rouding errors.
    return (*rDIV_RESULT + FX_DIV_HALF) >> FX_DIV_SHIFT;
}

void FX_InvAsync(fx32 x) {
    *rDIVCNT = DIVCNT_MODE_64_32;
    *rDIV_NUMER = (u64)FX32_ONE << 32;
    *rDIV_DENOM = (u32)x;
}

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

fx32 FX_GetSqrtResult(void) {
    while (*rSQRTCNT & SQRTCNTF_BUSY) {}

    // Shift result back to account for precision loss prevention.
    // Also add a half, to prevent rouding errors.
    return (*rSQRT_RESULT + FX_SQRT_HALF) >> FX_SQRT_SHIFT;
}

void FX_DivAsync(fx32 numer, fx32 denom) {
    *rDIVCNT = DIVCNT_MODE_64_32;

    // Shift numerator by 32 to avoid precision loss
    *rDIV_NUMER = (u64)numer << 32;
    *rDIV_DENOM = (u32)denom;
}

s32 FX_ModS32(s32 numer, s32 denom) {
    *rDIVCNT = DIVCNT_MODE_32_32;
    *rDIV_NUMER_L = (u32)numer;
    *rDIV_DENOM = (u32)denom;

    while (*rDIVCNT & DIVCNTF_BUSY) {}
    return *rDIVREM_RESULT_L;
}
