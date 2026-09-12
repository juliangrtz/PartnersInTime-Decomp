#include "save_write_effects_internal.h"

void SaveMenuWrite_StartEffects(void)
{
    SaveMenuTransferTask *task;
    WRITE_WORK.unknown_13f = 1;
    WRITE_WORK.panels[0].unknown_02 = 1;
    WRITE_WORK.panels[1].unknown_02 = 1;
    WRITE_WORK.panels[1].opacity = 0;
    WRITE_WORK.panels[1].scale = 8192;
    WRITE_WORK.panels[1].x = WRITE_WORK.panels[1].target_x + 0x40000;
    WRITE_WORK.panels[1].y = WRITE_WORK.panels[1].target_y - 0x60000;
    task = func_ov005_0206659c((void (*)(SaveMenuTransferTask *))func_ov008_0206b37c, 2, 1);
    task->arguments[0] = 1;
    task->counter = 0;
    task = func_ov005_0206659c((void (*)(SaveMenuTransferTask *))SaveMenuWrite_UpdateBackgroundZoom, 2, 1);
    task->arguments[0] = 1;
    task->counter = 0;
    func_02036988((vu16 *)0x04001050, 8, 31, 0, 16);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 2);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 3, 0);
    func_ov005_0206659c((void (*)(SaveMenuTransferTask *))SaveMenuWrite_UpdateBrightness, 2, 1);
    func_ov005_0206659c(func_ov008_0206ae78, 11, 1);
    func_ov008_020727d8(1, 12);
    func_ov005_02069bcc(367, 0, 0, 128);
}
