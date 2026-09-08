#include "save_menu_internal.h"

void SaveMenuText_QueueBackground(int engine, int background)
{
    SaveMenuTransferTask *task = func_ov005_0206659c(func_ov008_02075b94, 11, 1);
    task->arguments[0] = engine;
    task->arguments[1] = background;
}
