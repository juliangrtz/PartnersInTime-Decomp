#include <nitro.h>

extern u32 data_02059d90;
u32 func_02039a60(void);
int func_02039bac(void);
int func_02039a2c(void);

u32 func_02039b34(void) {
    if (data_02059d90 == 0xffffffff) {
        u32 cartridge = func_02039a60();
        u32 result;
        if (func_02039bac())
            result = cartridge | 0x10000000;
        else if (func_02039a2c())
            result = cartridge | 0x40000000;
        else if (cartridge & 0x01000000)
            result = cartridge | 0x20000000;
        else
            result = cartridge | 0x80000000;
        data_02059d90 = result | *(vu16 *)0x027ffffa;
    }
    return data_02059d90;
}
