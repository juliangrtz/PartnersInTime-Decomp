#include "save_menu_internal.h"

const u8 *SaveMenuText_GetString(SaveMenuText *context, int table, int entry)
{
    const u8 *localized =
        context->strings[table] + (((const u32 *)context->strings[table])[gSaveData[1301]] & ~3);
    return localized + ((const u32 *)localized)[entry];
}

u16 SaveMenuText_MeasureWidth(SaveMenuText *context, int table, int entry)
{
    GameTextBounds bounds;
    GameText_Init(&context->text, context->fonts, context->pixels, 0, 0, 0, 1, 4, 1, 6, 31, 0, 255, 0);
    context->text.text = SaveMenuText_GetString(context, table, entry);
    context->text.cursor.bits.x = 0;
    context->text.cursor.bits.y = 0;
    GameText_MeasureBounds(&context->text, &bounds);
    return bounds.width;
}

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
