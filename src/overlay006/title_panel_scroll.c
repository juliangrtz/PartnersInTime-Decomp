#include <nitro.h>

#define REG32(address) (*(vu32 *)(address))

void TitleScene_SetPanelScroll(int displacement)
{
    unsigned left = (128 - displacement) & 511;
    unsigned right = (displacement - 128) & 511;
    REG32(0x04000018) = left | 0x01800000;
    REG32(0x04001018) = left | 0x00740000;
    REG32(0x0400001c) = right | 0x01800000;
    REG32(0x0400101c) = right | 0x00740000;
}
