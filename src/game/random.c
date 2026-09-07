#include <game/random.h>

extern u32 data_02060e04;

void Random_SetSeed(u32 seed)
{
    data_02060e04 = seed;
}

int Random_Next(void)
{
    u32 value = data_02060e04;
    u32 mixed;
    if (!value) value = 711800410;
    value += (value + (value << 2)) << 3;
    mixed = (value >> 16) | ((value & 0xffff) << 16);
    mixed = ((value + mixed) & 0x1fffe) << 15;
    data_02060e04 = mixed;
    return mixed >> 16;
}

int Random_NextModulo(int modulus)
{
    return (u16)(Random_Next() % modulus);
}
