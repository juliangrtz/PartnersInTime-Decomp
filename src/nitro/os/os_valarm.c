#include <nitro/os_alarm.h>

extern u16 data_0206300c;
extern void *data_02063010;
extern void *data_02063014;
extern u32 data_02063018[2];
extern u32 OS_DisableIrqMask(u32 mask);

void OS_InitVAlarm(void)
{
    if (data_0206300c) return;
    data_0206300c = 1;
    data_02063018[0] = 0;
    data_02063018[1] = 0;
    OS_DisableIrqMask(4);
    data_02063014 = 0;
    data_02063010 = 0;
}
