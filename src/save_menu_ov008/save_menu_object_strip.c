#include "save_menu_internal.h"

void SaveMenuText_UploadObjectStrip(SaveMenuTransferTask *task)
{
    int row, column, offset, width;
    u8 *destination, *pixels;
    width = task->arguments[4];
    pixels = (u8 *)data_ov008_0207828c->pixels;
    offset = task->arguments[1];
    column = task->arguments[3];
    row = task->arguments[2];
    destination = (u8 *)Overlay5Display_GetObjVram((u8)task->arguments[0]) + offset + (column << 8);
    row *= 2;
    GameResource_Move16(pixels + ((u32)(32 * (width * row + 4 * column)) & ~3u), destination, 128);
    GameResource_Move16(pixels + ((u32)(32 * (width * (row + 1) + 4 * column)) & ~3u), destination + 128,
                        128);
    func_ov005_0206650c(task);
}

void SaveMenuText_QueueObjectStrip(int engine, int offset, int row, int column, int width)
{
    SaveMenuTransferTask *task = func_ov005_0206659c(SaveMenuText_UploadObjectStrip, 11, 1);
    task->arguments[0] = engine;
    task->arguments[1] = offset;
    task->arguments[2] = row;
    task->arguments[3] = column;
    task->arguments[4] = width;
}
