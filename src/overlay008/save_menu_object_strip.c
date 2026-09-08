#include "save_menu_internal.h"

void SaveMenuText_QueueObjectStrip(int engine, int offset, int row, int column, int width)
{
    SaveMenuTransferTask *task = func_ov005_0206659c(func_ov008_02075c38, 11, 1);
    task->arguments[0] = engine;
    task->arguments[1] = offset;
    task->arguments[2] = row;
    task->arguments[3] = column;
    task->arguments[4] = width;
}
