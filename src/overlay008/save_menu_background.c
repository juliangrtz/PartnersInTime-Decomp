#include "save_menu_internal.h"

void SaveMenuText_UploadBackground(SaveMenuTransferTask *task)
{
    u32 *pixels = data_ov008_0207828c->pixels;
    void *destination =
        (u8 *)Overlay5DisplayBg_GetCharacterVram((u8)task->arguments[0], (u8)task->arguments[1]) + 32;
    func_0203b7b4(pixels, destination, 7488);
    func_ov005_0206650c(task);
}

void SaveMenuText_QueueBackground(int engine, int background)
{
    SaveMenuTransferTask *task = func_ov005_0206659c(SaveMenuText_UploadBackground, 11, 1);
    task->arguments[0] = engine;
    task->arguments[1] = background;
}
