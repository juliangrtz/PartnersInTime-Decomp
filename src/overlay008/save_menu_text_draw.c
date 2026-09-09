#include "save_menu_internal.h"
#include <game/save_menu_write.h>
#include <game/overlay005_resource.h>
extern SaveMenuBufferHeader data_0205e32c;
Overlay5Sprite *Overlay5ResourceB_Get(SaveMenuTransferTask *);
void func_ov005_02069084(void *, int);
void func_ov005_020663d8(int);
void MIi_CpuClearFast(u32, void *, u32);
void SaveMenuText_ClearDialogVram(SaveMenuTransferTask *);
void func_ov008_02074f28(SaveMenuText *);
void func_ov008_02074cfc(SaveMenuText *);
void func_ov008_020748b0(SaveMenuText *);
void func_ov008_020744d8(SaveMenuText *, int);
void func_ov008_0207431c(SaveMenuText *, int);

void MIi_CpuClear16(u16, void *, u32);

static inline void ClearGlyphs(void *destination)
{
    volatile u16 clear = 0;
    MIi_CpuClear16(clear, destination, 128);
}

int SaveMenuText_DrawObjectRows(SaveMenuText *context, int engine, int *offset, int table, u16 entry,
                                int width, int row)
{
    int lines, i;
    int columns;
    GameText_Init(&context->text, context->fonts, context->pixels, 0, 0, (u8)(row * 16), 1, 4, 1, 6,
                  (u8)(width / 2 - 1), 0, 255, 0);
    context->text.text = SaveMenuText_GetString(context, table, entry);
    context->text.style.bits.color = context->text.cursor.bits.base_color;
    columns = (u8)(width / 8);
    lines = 1;
    for (;;) {
        GameTextCursor *cursor = &context->text.cursor;
        const u8 *text;
        for (text = context->text.text; text[0] == 255 && text[1] == 0; text = context->text.text) {
            cursor->bits.x = 0;
            lines = (u8)(lines + 1);
            cursor->bits.y += 16;
            context->text.text += 2;
        }
        if (!*text)
            break;
        GameText_Next(&context->text, 0, 0);
    }
    for (i = 0; i < lines; ++i) {
        int column;
        for (column = 0; column < columns; ++column)
            SaveMenuText_QueueObjectStrip(engine, *offset, row + i, column, width / 2);
        *offset += 32 * width;
    }
    return lines;
}

int SaveMenuText_StreamObjectText(SaveMenuText *context, int engine, int *offset, int table, u16 entry)
{
    int first;
    int width;
    GameText_Init(&context->text, context->fonts, context->pixels, 0, 0, 0, 1, 0, 1, 6, 31, 0, 255, 0);
    context->text.text = SaveMenuText_GetString(context, table, entry);
    width = 0;
    context->text.style.bits.color = context->text.cursor.bits.base_color;
    first = context->text.cursor.bits.x / 8;
    for (;;) {
        const u8 *text = context->text.text;
        int cursor = context->text.cursor.bits.x / 8;
        if (first + 4 <= cursor || cursor < first || !*text) {
            u8 *destination = (u8 *)Overlay5Display_GetObjVram(engine) + *offset;
            GameResource_Move16(&context->pixels[32 * first / 4], destination, 128);
            GameResource_Move16(&context->pixels[32 * (first + 32) / 4], destination + 128, 128);
            *offset += 256;
            ClearGlyphs(&context->pixels[32 * first / 4]);
            ClearGlyphs(&context->pixels[32 * (first + 32) / 4]);
            first = (first + 4) % 32;
        }
        if (!*text)
            break;
        {
            u32 previous = context->text.cursor.bits.x;
            GameText_Next(&context->text, 0, 0);
            width += context->text.cursor.bits.x - previous;
        }
        if (context->text.cursor.bits.x >= 256) {
            GameResource_Move16(context->pixels + 256, context->pixels, 128);
            GameResource_Move16(context->pixels + 512, context->pixels + 256, 128);
            ClearGlyphs(context->pixels + 512);
            context->text.cursor.bits.x -= 256;
        }
    }
    return (u16)width;
}

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

void SaveMenuText_BuildDialog(SaveMenuText *text, int kind, int entry)
{
    switch (kind) {
    case 0:
        func_ov008_02074f28(text);
        break;
    case 1:
        func_ov008_02074cfc(text);
        break;
    case 2:
        func_ov008_020748b0(text);
        break;
    case 3:
        func_ov008_020744d8(text, entry);
        break;
    case 4:
        func_ov008_0207431c(text, entry);
        break;
    }
}
void SaveMenuText_ClearDialogVram(SaveMenuTransferTask *task)
{
    void *destination = Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN);
    volatile u32 clear = 0;
    MIi_CpuClearFast(clear, destination, 6144);
    func_ov005_0206650c(task);
}
void SaveMenuText_ResetDialogSprites(void)
{
    func_ov005_020663d8(3);
    func_ov005_0206659c(SaveMenuText_ClearDialogVram, 11, 1);
}
void SaveMenuText_DrawSlotChoice(SaveMenuTransferTask *task)
{
    Overlay5Sprite *sprite = Overlay5ResourceB_Get(task);
    if ((u32)(task->arguments[0] - 1) <= 1) {
        if (data_0205e32c.settings.occupied_slots & (1 << ((s8 *)data_ov008_02078290)[0x19d]))
            sprite->attributes_1 &= ~0xf000;
        else
            sprite->attributes_1 = (sprite->attributes_1 & ~0xf000) | 0x1000;
    }
    func_ov005_02069084(sprite, 60);
}
