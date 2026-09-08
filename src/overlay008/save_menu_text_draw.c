#include "save_menu_internal.h"

void SaveMenuText_DrawBackground(SaveMenuText *context, int engine, int background, int table, u16 entry,
                                 int x, int y)
{
    GameTextToken token;
    volatile u32 clear;
    GameText_Init(&context->text, context->fonts, context->pixels, 0, 0, 0, 1, 3, 1, 6, 25, 0, 255, 0);
    context->text.text = SaveMenuText_GetString(context, table, entry);
    context->text.style.bits.color = context->text.cursor.bits.base_color;
    clear = 0;
    func_0203b7a0(clear, context->pixels, 7488);
    context->text.cursor.bits.x = x;
    context->text.cursor.bits.y = y;
    for (;;) {
        if (!*context->text.text)
            break;
        token = GameText_Next(&context->text, 0, 0);
    }
    SaveMenuText_QueueBackground(engine, background);
}

int SaveMenuText_DrawTextureRows(SaveMenuText *context, int *offset, int table, int entry, int width, int row)
{
    int lines, i;
    GameText_Init(&context->text, context->fonts, context->pixels, 0, 0, row * 16, 1, 4, 1, 6, width / 4 - 1,
                  0, 255, 0);
    context->text.text = SaveMenuText_GetString(context, table, entry);
    context->text.style.bits.color = context->text.cursor.bits.base_color;
    lines = 1;
    for (;;) {
        if (!*context->text.text)
            break;
        if (context->text.text[0] == 255 && context->text.text[1] == 0)
            lines = (u8)(lines + 1);
        GameText_Next(&context->text, 0, 0);
    }
    for (i = 0; i < lines; ++i) {
        SaveMenuText_QueueTextureRow(*offset, width, row + i);
        *offset += 16 * width;
    }
    return lines;
}
