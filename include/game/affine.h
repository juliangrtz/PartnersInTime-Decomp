#ifndef PIT_GAME_AFFINE_H
#define PIT_GAME_AFFINE_H
#include <nitro.h>

/* Two-by-two matrix with eight fractional bits, stored in row order. */
typedef struct AffineMatrixQ8 {
    union {
        struct { s16 m00, m01, m10, m11; };
        s16 values[4];
        u32 rows[2];
    };
} AffineMatrixQ8;

#ifdef __cplusplus
extern "C" {
#endif
/* Returns -1 for a zero determinant, leaving the destination untouched. */
int GameAffine_InvertQ8(const AffineMatrixQ8 *source, AffineMatrixQ8 *destination);
#ifdef __cplusplus
}
#endif
#endif
