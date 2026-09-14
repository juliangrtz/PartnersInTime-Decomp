#include <nitro/tp.h>
extern void OS_Terminate(void);
typedef union TpPackedSample {
    u16 halves[2];
    struct {
        u32 x : 12;
        u32 y : 12;
        u32 touch : 1;
        u32 validity : 2;
        u32 reserved : 5;
    } bits;
} TpPackedSample;
typedef char TpPackedSampleSizeCheck[(sizeof(TpPackedSample) == 4) ? 1 : -1];

/* ARM7 publishes a packed sample as two halfwords in shared RAM. */
static inline void ReadSample(TpData *out, int reset) {
    TpPackedSample packed;
    packed.halves[0] = *(u16 *)0x027fffaa;
    packed.halves[1] = *(u16 *)0x027fffac;
    if (reset)
        data_02064c8c.state = 0;
    out->x = packed.bits.x;
    out->y = packed.bits.y;
    out->touch = (u8)packed.bits.touch;
    out->validity = (u8)packed.bits.validity;
}
void TPi_FifoCallback(u32 tag, u32 data, int error) {
    u16 packet = (u16)data;
    u16 command = (packet & 0x7f00) >> 8;
    if (error) {
        data_02064c8c.errors |= 1 << command;
        if (data_02064c8c.callback)
            data_02064c8c.callback(command, 4, 0);
        return;
    }
    if (command == 16) {
        data_02064c8c.index++;
        if (data_02064c8c.index >= data_02064c8c.count)
            data_02064c8c.index = 0;
        ReadSample(&data_02064c8c.buffer[data_02064c8c.index], 0);
        if (data_02064c8c.callback)
            data_02064c8c.callback(command, 0, (u8)data_02064c8c.index);
        return;
    }
    if (data & 0x1000000) {
        int result;
        switch ((u8)packet) {
        case 0:
            switch (command) {
            case 0:
                ReadSample(&data_02064c8c.sample, 1);
                break;
            case 1:
                data_02064c8c.state = 2;
                break;
            case 2:
                data_02064c8c.state = 0;
                break;
            }
            data_02064c8c.busy &= ~(1 << command);
            if (data_02064c8c.callback)
                data_02064c8c.callback(command, 0, 0);
            return;
        case 4:
            result = 3;
            goto command_error;
        case 2:
            result = 1;
            goto command_error;
        case 3:
            result = 2;
        command_error:
            data_02064c8c.errors |= 1 << command;
            data_02064c8c.busy &= ~(1 << command);
            if (data_02064c8c.callback)
                data_02064c8c.callback(command, (u8)result, 0);
            break;
        default:
            OS_Terminate();
            break;
        }
    }
}
