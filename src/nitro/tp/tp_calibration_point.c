#include <nitro/tp.h>

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
