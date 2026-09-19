/*
 * Touch panel driver (Nitro SDK, 0x02040678-0x02041270).
 *
 * Sampling runs on the ARM7: the ARM9 sends a request over PXI, the reply
 * arrives in the FIFO callback, and the sample is published as two packed
 * halfwords in shared RAM. Raw samples are in panel units; calibration turns
 * them into screen pixels using the two reference points stored in the firmware,
 * and CalcCalibrateParam is what derives that mapping.
 */

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

extern u16 data_02064c88;
extern void PXI_Init(void);
extern int PXI_IsCallbackReady(u32 tag, int processor);
extern void PXI_SetFifoRecvCallback(u32 tag, void (*callback)(u32, u32, int));
extern int PXI_SendWordByFifo(u32 tag, u32 data, int error);

#define TP_DIVCNT (*(vu16 *)0x04000280)
#define TP_NUMERATOR (*(vu32 *)0x04000290)
#define TP_DENOMINATOR (*(volatile u64 *)0x04000298)
#define TP_QUOTIENT (*(volatile s32 *)0x040002A0)

static inline int TPi_Send(u32 data) {
    if (PXI_SendWordByFifo(6, data, 0) >= 0) return 1;
    return 0;
}

static inline int TPi_SendStart(u16 scanline, u16 frequency) {
    if (PXI_SendWordByFifo(6, (u8)frequency | 0x02000100, 0) < 0) return 0;
    if (PXI_SendWordByFifo(6, scanline | 0x01010000, 0) < 0) return 0;
    return 1;
}

void TP_Init(void) {
    if (data_02064c88) return;
    data_02064c88 = 1;
    PXI_Init();
    data_02064c8c.state = 0;
    data_02064c8c.busy = 0;
    data_02064c8c.index = 0;
    data_02064c8c.callback = 0;
    data_02064c8c.buffer = 0;
    data_02064c8c.calibrated = 0;
    data_02064c8c.errors = 0;
    while (!PXI_IsCallbackReady(6, 1)) {}
    PXI_SetFifoRecvCallback(6, TPi_FifoCallback);
}

int TP_GetUserInfo(TpCalibration *calibration) {
    typedef struct TpUserInfo {
        u8 unknown[0x58];
        u16 raw_x1, raw_y1;
        u8 display_x1, display_y1;
        u16 raw_x2, raw_y2;
        u8 display_x2, display_y2;
    } TpUserInfo;
    const TpUserInfo *info = (const TpUserInfo *)0x027FFC80;
    u16 raw_x1 = info->raw_x1;
    u16 raw_y1 = info->raw_y1;
    u16 display_x1 = info->display_x1;
    u16 raw_x2;
    u16 raw_y2;
    u16 display_y1;
    u16 display_x2;
    u16 display_y2;
    display_y1 = info->display_y1;
    raw_x2 = info->raw_x2;
    raw_y2 = info->raw_y2;
    display_x2 = info->display_x2;
    display_y2 = info->display_y2;
    if (!(raw_x1 || raw_x2 || raw_y1 || raw_y2) ||
        TP_CalcCalibrateParam(calibration, raw_x1, raw_y1, display_x1, display_y1,
            raw_x2, raw_y2, display_x2, display_y2)) {
        calibration->x_offset = 0;
        calibration->y_offset = 0;
        calibration->x_step = 0;
        calibration->y_step = 0;
        return 1;
    }
    return 1;
}

void TP_SetCalibrateParam(const TpCalibration *calibration) {
    u32 state;
    s32 x_step;
    s32 y_step;
    if (!calibration) { data_02064c8c.calibrated = 0; return; }
    state = OS_DisableInterrupts();
    x_step = calibration->x_step;
    if (x_step) {
        TP_DIVCNT = 0;
        TP_NUMERATOR = 0x10000000;
        TP_DENOMINATOR = (u32)x_step;
        data_02064c8c.calibration[0].origin = calibration->x_offset;
        data_02064c8c.calibration[0].step = calibration->x_step;
        while (TP_DIVCNT & 0x8000) {}
        data_02064c8c.calibration[0].scale = TP_QUOTIENT;
    } else {
        data_02064c8c.calibration[0].origin = 0;
        data_02064c8c.calibration[0].step = 0;
        data_02064c8c.calibration[0].scale = 0;
    }
    y_step = calibration->y_step;
    if (y_step) {
        TP_DIVCNT = 0;
        TP_NUMERATOR = 0x10000000;
        TP_DENOMINATOR = (u32)y_step;
        data_02064c8c.calibration[1].origin = calibration->y_offset;
        data_02064c8c.calibration[1].step = calibration->y_step;
        while (TP_DIVCNT & 0x8000) {}
        data_02064c8c.calibration[1].scale = TP_QUOTIENT;
    } else {
        data_02064c8c.calibration[1].origin = 0;
        data_02064c8c.calibration[1].step = 0;
        data_02064c8c.calibration[1].scale = 0;
    }
    OS_RestoreInterrupts(state);
    data_02064c8c.calibrated = 1;
}

void TP_RequestSamplingAsync(void) {
    u32 state = OS_DisableInterrupts();
    if (!TPi_Send(0x03000000)) {
        OS_RestoreInterrupts(state);
        data_02064c8c.errors |= 1;
        if (data_02064c8c.callback) data_02064c8c.callback(0, 4, 0);
    } else {
        data_02064c8c.busy |= 1;
        data_02064c8c.errors &= ~1;
        OS_RestoreInterrupts(state);
    }
}

int TP_GetCalibratedResult(TpData *output) {
    if (data_02064c8c.errors & 1) return 1;
    *output = data_02064c8c.sample;
    TP_GetCalibratedPoint(output, output);
    return 0;
}
int TP_WaitCalibratedResult(TpData *output) {
    TP_WaitBusy(1);
    return TP_GetCalibratedResult(output);
}

void TP_RequestAutoSamplingStartAsync(u16 scanline, u16 frequency, TpData *buffer, u16 count) {
    u32 i;
    u32 state;
    data_02064c8c.buffer = buffer;
    data_02064c8c.index = 0;
    data_02064c8c.frequency = frequency;
    data_02064c8c.count = count;
    for (i = 0; i < count; i++) data_02064c8c.buffer[i].touch = 0;
    state = OS_DisableInterrupts();
    if (!(u8)TPi_SendStart(scanline, frequency)) {
        OS_RestoreInterrupts(state);
        data_02064c8c.errors |= 2;
        if (data_02064c8c.callback) data_02064c8c.callback(1, 4, 0);
    } else {
        data_02064c8c.busy |= 2;
        data_02064c8c.errors &= ~2;
        OS_RestoreInterrupts(state);
    }
}

void TP_RequestAutoSamplingStopAsync(void) {
    u32 state = OS_DisableInterrupts();
    if (!TPi_Send(0x03000200)) {
        OS_RestoreInterrupts(state);
        data_02064c8c.errors |= 4;
        if (data_02064c8c.callback) data_02064c8c.callback(2, 4, 0);
    } else {
        data_02064c8c.busy |= 4;
        data_02064c8c.errors &= ~4;
        OS_RestoreInterrupts(state);
    }
}
u16 TP_GetLatestIndexInAuto(void) { return data_02064c8c.index; }


int TP_CalcCalibrateParam(TpCalibration *output, u16 raw_x1, u16 raw_y1,
    u16 display_x1, u16 display_y1, u16 raw_x2, u16 raw_y2, u16 display_x2, u16 display_y2)
{
    u32 interrupts;
    int x_step;
    int y_step;
    int offset;
    s32 raw_difference_y;
    s32 display_difference_y;
    if (raw_x1 >= 0x1000 || raw_y1 >= 0x1000 || raw_x2 >= 0x1000 || raw_y2 >= 0x1000) return 1;
    if (display_x1 >= 0x100 || display_x2 >= 0x100 || display_y1 >= 0xc0 || display_y2 >= 0xc0) return 1;
    if (display_x1 == display_x2 || display_y1 == display_y2 || raw_x1 == raw_x2 || raw_y1 == raw_y2) return 1;
    interrupts = OS_DisableInterrupts();
    TP_DIVCNT = 0;
    TP_NUMERATOR = (raw_x1 - raw_x2) << 8;
    TP_DENOMINATOR = (u32)(display_x1 - display_x2);
    raw_difference_y = raw_y1 - raw_y2;
    display_difference_y = display_y1 - display_y2;
    while (TP_DIVCNT & 0x8000) {}
    x_step = TP_QUOTIENT;
    TP_DIVCNT = 0;
    TP_NUMERATOR = raw_difference_y << 8;
    TP_DENOMINATOR = (u32)display_difference_y;
    if (x_step >= 0x8000 || x_step < -0x8000) {
        OS_RestoreInterrupts(interrupts);
        return 1;
    }
    output->x_step = x_step;
    offset = (s16)((((raw_x1 + raw_x2) << 8) - output->x_step * (display_x1 + display_x2)) >> 7);
    if (offset >= 0x8000 || offset < -0x8000) {
        OS_RestoreInterrupts(interrupts);
        return 1;
    }
    output->x_offset = offset;
    while (TP_DIVCNT & 0x8000) {}
    y_step = TP_QUOTIENT;
    OS_RestoreInterrupts(interrupts);
    if (y_step >= 0x8000 || y_step < -0x8000) return 1;
    output->y_step = y_step;
    offset = (s16)((((raw_y1 + raw_y2) << 8) - output->y_step * (display_y1 + display_y2)) >> 7);
    if (offset >= 0x8000 || offset < -0x8000) return 1;
    output->y_offset = offset;
    return 0;
}

extern TpAxisCalibration data_02064ca4[2];

void TP_GetCalibratedPoint(TpData *output, const TpData *raw) {
    TpAxisCalibration *calibration;
    if (!data_02064c8c.calibrated) { *output = *raw; return; }
    calibration = data_02064ca4;
    output->touch = raw->touch;
    output->validity = raw->validity;
    if (!raw->touch) { output->x = 0; output->y = 0; return; }
    output->x = (calibration[0].scale * ((s64)(raw->x << 2) - calibration[0].origin)) >> 22;
    if ((s16)output->x < 0) output->x = 0;
    else if ((s16)output->x > 255) output->x = 255;
    output->y = (calibration[1].scale * ((s64)(raw->y << 2) - calibration[1].origin)) >> 22;
    if ((s16)output->y < 0) output->y = 0;
    else if ((s16)output->y > 191) output->y = 191;
}

void TP_WaitBusy(u16 mask) { while (data_02064c8c.busy & mask) {} }
u32 TP_CheckError(u16 mask) { return data_02064c8c.errors & mask; }
