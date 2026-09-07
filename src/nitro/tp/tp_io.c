#include <nitro/tp.h>

extern u16 data_02064c88;
extern void func_0203d3ac(void);
extern int func_0203d590(u32 tag, int processor);
extern void func_0203d5b8(u32 tag, void (*callback)(u32, u32, int));
extern int func_0203d4dc(u32 tag, u32 data, int error);

#define TP_DIVCNT (*(vu16 *)0x04000280)
#define TP_NUMERATOR (*(vu32 *)0x04000290)
#define TP_DENOMINATOR (*(volatile u64 *)0x04000298)
#define TP_QUOTIENT (*(volatile s32 *)0x040002A0)

static inline int TPi_Send(u32 data) {
    if (func_0203d4dc(6, data, 0) >= 0) return 1;
    return 0;
}

static inline int TPi_SendStart(u16 scanline, u16 frequency) {
    if (func_0203d4dc(6, (u8)frequency | 0x02000100, 0) < 0) return 0;
    if (func_0203d4dc(6, scanline | 0x01010000, 0) < 0) return 0;
    return 1;
}

void TP_Init(void) {
    if (data_02064c88) return;
    data_02064c88 = 1;
    func_0203d3ac();
    data_02064c8c.state = 0;
    data_02064c8c.busy = 0;
    data_02064c8c.index = 0;
    data_02064c8c.callback = 0;
    data_02064c8c.buffer = 0;
    data_02064c8c.calibrated = 0;
    data_02064c8c.errors = 0;
    while (!func_0203d590(6, 1)) {}
    func_0203d5b8(6, TPi_FifoCallback);
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

