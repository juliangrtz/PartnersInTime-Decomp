#include <game/affine.h>

int GameAffine_InvertQ8(const AffineMatrixQ8 *source, AffineMatrixQ8 *destination)
{
    int m11 = source->m11;
    int determinant = source->m00 * m11 - source->m01 * source->m10;
    int divisor;
    if (determinant == 0)
        return -1;
    divisor = (determinant + 128) >> 8;
    if (divisor == 0) {
        /* The original saturates each source component independently when
           rounding loses the nonzero determinant. Preserve that edge case. */
        destination->m00 = m11 ? (m11 < 0 ? 0x8000 : 32767) : 0;
        destination->m01 = source->m01 ? (source->m01 < 0 ? 0x8000 : 32767) : 0;
        destination->m10 = source->m10 ? (source->m10 < 0 ? 0x8000 : 32767) : 0;
        destination->m11 = source->m00 ? (source->m00 < 0 ? 0x8000 : 32767) : 0;
        return 0;
    }
    /* Compute m00 in hardware while the other three divisions use the
       software helper. The two completion polls are present in the original. */
    *(volatile u16 *)0x04000280 = 0;
    *(volatile s32 *)0x04000290 = m11 << 8;
    *(volatile u64 *)0x04000298 = (u32)divisor;
    destination->m01 = -((source->m01 << 8) / divisor);
    destination->m10 = -((source->m10 << 8) / divisor);
    destination->m11 = (source->m00 << 8) / divisor;
    while (*(volatile u16 *)0x04000280 & 0x8000) {}
    while (*(volatile u16 *)0x04000280 & 0x8000) {}
    destination->m00 = *(volatile s32 *)0x040002a0;
    return 0;
}
