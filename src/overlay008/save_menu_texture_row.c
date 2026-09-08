#include "save_menu_internal.h"

void SaveMenuText_QueueTextureRow(int offset, int width, int row)
{
    SaveMenuTransferTask *task = func_ov005_0206659c(func_ov008_02075ac8, 11, 1);
    task->arguments[0] = offset;
    task->arguments[1] = width;
    task->arguments[2] = row;
}
