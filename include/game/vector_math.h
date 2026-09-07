#ifndef PIT_GAME_VECTOR_MATH_H
#define PIT_GAME_VECTOR_MATH_H
#include <nitro.h>
int GameVector_CountBits(const s32 *vector);
int GameVector_ScaleToBits(s32 *vector, int bits);
int GameVector_ScalePair(s32 *first, s32 *second, int bits);
void GameVector_CrossProduct(s32 *result, const s32 *first, const s32 *second, const s32 *origin);
int GameVector_Cosine(s32 *first, s32 *second);
#endif
