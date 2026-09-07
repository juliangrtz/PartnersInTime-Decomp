#ifndef PIT_GAME_RANDOM_H
#define PIT_GAME_RANDOM_H

#include <nitro.h>

void Random_SetSeed(u32 seed);
int Random_Next(void);
int Random_NextModulo(int modulus);

#endif
