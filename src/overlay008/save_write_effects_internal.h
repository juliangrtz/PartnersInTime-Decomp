#ifndef PIT_SAVE_WRITE_EFFECTS_INTERNAL_H
#define PIT_SAVE_WRITE_EFFECTS_INTERNAL_H
#include "save_menu_internal.h"

typedef struct SaveWriteBrightnessTask {
    u8 unknown_00[32];
    int phase, timer, amount;
    u8 unknown_2c[28];
} SaveWriteBrightnessTask;

typedef struct SaveWriteZoomTask {
    u8 unknown_00[32];
    int phase, timer, unknown_28, scale, blend;
    u8 unknown_34[20];
} SaveWriteZoomTask;

void SaveMenuWrite_UpdateBrightness(SaveWriteBrightnessTask *task);
void SaveMenuWrite_UpdateBackgroundZoom(SaveWriteZoomTask *task);

typedef char SaveWriteBrightnessTaskSizeCheck[
    sizeof(SaveWriteBrightnessTask) == 72 ? 1 : -1];
typedef char SaveWriteZoomTaskSizeCheck[
    sizeof(SaveWriteZoomTask) == 72 ? 1 : -1];
#endif
