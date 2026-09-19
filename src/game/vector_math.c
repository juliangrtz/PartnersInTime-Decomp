#include <game/vector_math.h>

static inline int GameVector_LeadingZeros(u32 value) {
    /* ARM leading-zero count; the surrounding vector arithmetic is C. */
    asm { clz value, value }
    return value;
}
int GameVector_CountBits(const s32 *vector) {
    int minimum = 31;
    int value;
    int zeros;
    value = vector[0];
    if (value < 0) value = -value;
    zeros = GameVector_LeadingZeros(value);
    if (zeros < minimum) minimum = zeros;
    value = vector[1];
    if (value < 0) value = -value;
    zeros = GameVector_LeadingZeros(value);
    if (zeros < minimum) minimum = zeros;
    value = vector[2];
    if (value < 0) value = -value;
    zeros = GameVector_LeadingZeros(value);
    if (zeros < minimum) minimum = zeros;
    return 32 - minimum;
}
int GameVector_ScaleToBits(s32 *vector, int bits) {
    bits -= GameVector_CountBits(vector);
    if (bits > 0) {
        vector[0] <<= bits; vector[1] <<= bits; vector[2] <<= bits;
    } else if (bits < 0) {
        vector[0] >>= -bits; vector[1] >>= -bits; vector[2] >>= -bits;
    }
    return bits;
}
int GameVector_ScalePair(s32 *first, s32 *second, int bits) {
    int first_bits = GameVector_CountBits(first);
    int second_bits = GameVector_CountBits(second);
    int difference = bits - (first_bits + second_bits);
    int total = 0;
    if (difference > 0) {
        int shift = (difference + 1) / 2;
        total = 2 * shift;
        first[0] <<= shift; first[1] <<= shift; first[2] <<= shift;
        second[0] <<= shift; second[1] <<= shift; second[2] <<= shift;
    } else if (difference < 0) {
        int shift = 2 * ((difference - 1) / 2);
        total = shift;
        while (shift) {
            if (first_bits < second_bits) {
                --second_bits;
                second[0] >>= 1; second[1] >>= 1; second[2] >>= 1;
            } else {
                --first_bits;
                first[0] >>= 1; first[1] >>= 1; first[2] >>= 1;
            }
            ++shift;
        }
    }
    return total;
}
void GameVector_CrossProduct(s32 *result, const s32 *first, const s32 *second, const s32 *origin) {
    s32 a[3], b[3];
    if (origin) {
        a[0] = first[0] - origin[0]; a[1] = first[1] - origin[1]; a[2] = first[2] - origin[2];
        b[0] = second[0] - origin[0]; b[1] = second[1] - origin[1]; b[2] = second[2] - origin[2];
    } else {
        a[0] = first[0]; a[1] = first[1]; a[2] = first[2];
        b[0] = second[0]; b[1] = second[1]; b[2] = second[2];
    }
    GameVector_ScalePair(a, b, 30);
    result[0] = a[1] * b[2] - b[1] * a[2];
    result[1] = a[2] * b[0] - b[2] * a[0];
    result[2] = a[0] * b[1] - b[0] * a[1];
}

extern s32 data_0205a8d8[3];
extern s32 data_0205a8cc[3];
extern s32 data_0205a8e4[3];
static inline void GameVector_Sqrt(u32 square) {
    *(vu16 *)0x040002b0 = 0;
    *(vu32 *)0x040002b8 = square;
    while (*(vu16 *)0x040002b0 & 0x8000) {}
}
int GameVector_Cosine(s32 *first, s32 *second) {
    int shift;
    int result;
    GameVector_ScaleToBits(first, 14);
    GameVector_ScaleToBits(second, 14);
    data_0205a8d8[0] = first[0] * first[0];
    data_0205a8d8[1] = first[1] * first[1];
    data_0205a8d8[2] = first[2] * first[2];
    data_0205a8cc[0] = second[0] * second[0];
    data_0205a8cc[1] = second[1] * second[1];
    data_0205a8cc[2] = second[2] * second[2];
    data_0205a8e4[0] = first[0] * second[0];
    data_0205a8e4[1] = first[1] * second[1];
    data_0205a8e4[2] = first[2] * second[2];
    shift = GameVector_ScalePair(data_0205a8d8, data_0205a8cc, 24) / 2 + 12;
    if (shift > 0) {
        data_0205a8e4[0] <<= shift; data_0205a8e4[1] <<= shift; data_0205a8e4[2] <<= shift;
    } else {
        shift = -shift;
        data_0205a8e4[0] >>= shift; data_0205a8e4[1] >>= shift; data_0205a8e4[2] >>= shift;
    }
    GameVector_Sqrt((data_0205a8d8[0] + data_0205a8d8[1] + data_0205a8d8[2]) *
        (data_0205a8cc[0] + data_0205a8cc[1] + data_0205a8cc[2]));
    result = (data_0205a8e4[0] + data_0205a8e4[1] + data_0205a8e4[2]) / *(volatile s32 *)0x040002b4;
    if (result > 4096) result = 4096;
    if (result < -4096) result = -4096;
    return result;
}
