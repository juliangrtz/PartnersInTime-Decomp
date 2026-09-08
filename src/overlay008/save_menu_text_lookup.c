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
