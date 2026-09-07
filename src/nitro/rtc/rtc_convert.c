#include <nitro/rtc.h>

u32 RTCi_ConvertBcdToBinary(u32 value) {
    u32 result = 0;
    int i = 0;
    int shift = 0;
    u32 power;
    int bit;
    int digit;
    do {
        if (((value >> shift) & 15) >= 10) return 0;
        i++;
        shift += 4;
    } while (i < 8);
    digit = 0;
    bit = 0;
    power = 1;
    do {
        result += power * ((value >> bit) & 15);
        digit++;
        power *= 10;
        bit += 4;
    } while (digit < 8);
    return result;
}

void RTCi_SetSyncResult(u32 result, void *argument) { data_02064d24.sync_result = result; }
