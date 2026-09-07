#include <nns/snd.h>

extern u32 data_02060ebc;
extern u32 data_02060ec0;
extern u32 data_02060ec4;
extern void func_0203bfc8(u32 mask, u32 flags);
extern void func_0203bff4(u32 mask, u32 flags);

int NNSi_SndLockChannel(u32 mask)
{
    if (!mask) {
        return 1;
    }
    if (mask & data_02060ec4) {
        return 0;
    }
    func_0203bff4(mask, 0);
    data_02060ec4 |= mask;
    return 1;
}

void NNSi_SndUnlockChannel(u32 mask)
{
    if (!mask) {
        return;
    }
    func_0203bfc8(mask, 0);
    data_02060ec4 &= ~mask;
}

int NNSi_SndAllocCapture(u32 mask)
{
    if (mask & data_02060ebc) {
        return 0;
    }
    data_02060ebc |= mask;
    return 1;
}

void NNSi_SndFreeCapture(u32 mask)
{
    data_02060ebc &= ~mask;
}

int NNSi_SndAllocAlarm(void)
{
    int index;
    u32 bit = 1;
    for (index = 0; index < 8; ++index) {
        if (!(data_02060ec0 & bit)) {
            data_02060ec0 |= bit;
            return index;
        }
        bit <<= 1;
    }
    return -1;
}

void NNSi_SndFreeAlarm(int alarm)
{
    data_02060ec0 &= ~(1 << alarm);
}

void NNSi_SndInitResource(void)
{
    data_02060ec4 = 0;
    data_02060ebc = 0;
    data_02060ec0 = 0;
}
