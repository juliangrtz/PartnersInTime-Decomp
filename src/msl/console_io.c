#include <nitro.h>

extern int func_02046fd4(void);
extern void func_02046fc0(const u8 *character);

int MSL_ReadConsole(int handle, u8 *buffer, u32 *length, void *refcon)
{
    u32 index;
    u32 count = *length;
    index = 0;
    for (; index < count; ++index) {
        buffer[index] = func_02046fd4();
        if (buffer[index] == '\r' || buffer[index] == '\n') {
            *length = index + 1;
            break;
        }
    }
    return 0;
}

int MSL_WriteConsole(int handle, const u8 *buffer, u32 *length, void *refcon)
{
    u32 index;
    u32 count = *length;
    index = 0;
    for (; index < count; ++index) {
        func_02046fc0(buffer + index);
    }
    return 0;
}
