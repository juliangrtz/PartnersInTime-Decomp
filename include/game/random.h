#ifndef PIT_GAME_RANDOM_H
#define PIT_GAME_RANDOM_H

/*
 * The game's random generator. The seed is shared, so everything that draws from
 * it within a frame advances the same sequence.
 */

#include <nitro.h>

void Random_SetSeed(u32 seed);
int Random_Next(void);
int Random_NextModulo(int modulus);

#endif
