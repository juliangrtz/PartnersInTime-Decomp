#include <nitro.h>

extern u32 data_02063028;
extern u16 data_0206302c[9];

void OSi_InitVramExclusive(void)
{
    int bank;
    data_02063028 = 0;
    for (bank = 0; bank < 9; ++bank) data_0206302c[bank] = 0;
}
