#ifndef NITRO_TP_H
#define NITRO_TP_H

#include <nitro/os_sync.h>

typedef struct TpData { u16 x, y, touch, validity; } TpData;
typedef struct TpCalibration { s16 x_offset, y_offset, x_step, y_step; } TpCalibration;
typedef void (*TpCallback)(u32 command, u32 result, u32 sample);
typedef struct TpAxisCalibration { s32 origin, step, scale; } TpAxisCalibration;
typedef struct TpWork {
    TpCallback callback;
    TpData sample;
    volatile u16 index;
    u16 frequency;
    TpData *buffer;
    u16 count;
    u16 padding16;
    TpAxisCalibration calibration[2];
    u16 calibrated;
    volatile u16 state;
    volatile u16 errors;
    volatile u16 busy;
} TpWork;

typedef char TpWorkSizeCheck[(sizeof(TpWork) == 56) ? 1 : -1];
typedef char TpDataSizeCheck[(sizeof(TpData) == 8) ? 1 : -1];

extern TpWork data_02064c8c;
void TP_GetCalibratedPoint(TpData *output, const TpData *raw);
int TP_CalcCalibrateParam(TpCalibration *output, u16 raw_x1, u16 raw_y1,
    u16 display_x1, u16 display_y1, u16 raw_x2, u16 raw_y2, u16 display_x2, u16 display_y2);
void TP_SetCalibrateParam(const TpCalibration *calibration);
int TP_GetUserInfo(TpCalibration *calibration);
void TP_Init(void);
void TPi_FifoCallback(u32 tag, u32 data, int error);
void TP_RequestSamplingAsync(void);
int TP_GetCalibratedResult(TpData *output);
int TP_WaitCalibratedResult(TpData *output);
void TP_RequestAutoSamplingStartAsync(u16 scanline, u16 frequency, TpData *buffer, u16 count);
void TP_RequestAutoSamplingStopAsync(void);
u16 TP_GetLatestIndexInAuto(void);
void TP_WaitBusy(u16 mask);
u32 TP_CheckError(u16 mask);

#endif
