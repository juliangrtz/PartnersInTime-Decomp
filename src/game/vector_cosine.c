#include <game/vector_math.h>
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
