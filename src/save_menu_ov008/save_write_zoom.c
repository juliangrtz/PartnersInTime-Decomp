#include "save_write_effects_internal.h"

void SaveMenuWrite_UpdateBackgroundZoom(SaveWriteZoomTask *task)
{
    switch (task->phase) {
    case 0:
        if (task->timer) {
            --task->timer;
            break;
        }
        task->scale = 8192;
        task->blend = 0;
        task->timer = 0;
        ++task->phase;
        /* Fall through to the first scale and blend update. */
    case 1: {
        int blend;
        task->scale -= 256;
        task->blend += 4096;
        blend = task->blend / 4096;
        if (blend > 16) blend = 16;
        REG16(0x04001052) = blend | ((16 - blend) << 8);
        if (task->scale <= 4096) {
            task->scale = 4096;
            task->blend = 65536;
            REG16(0x04001050) = 0;
            Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_SUB, 3, 4096, 4096, 0, 0, 0, 0, 0);
            func_ov005_0206650c((SaveMenuTransferTask *)task);
            return;
        }
        break;
    }
    }
    Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_SUB, 3, task->scale, task->scale, 0, 128, 76, 0, 0);
}
