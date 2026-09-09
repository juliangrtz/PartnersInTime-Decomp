extern "C" {
#include <game/battle_text.h>
#include <game/battle_context.h>
void func_0202cbd4(void *, int, u32);
int BattleInterfaceLayer_QueueResource(BattleInterfaceLayer *, const void *, int, int, int, int);
}

enum { BATTLE_TEXT_FONT_TABLE_OFFSET = 26868 };

/* Both variants share the text and resource-queue prefix through byte 51. */
extern "C" void BattleClippedText_QueueResource(BattleClippedText *text, const void *resource, int clear,
                                                s16 value)
{
    text->allocation_size = text->width * 32 * text->height;
    BattleInterfaceLayer_QueueResource((BattleInterfaceLayer *)text, resource, text->horizontal_mode,
                                       text->vertical_mode, 0, clear);
    text->flags.bits.requested = 1;
    text->unknown_3c = value;
}
extern "C" void BattleTiledText_QueueResource(BattleTiledText *text, const void *resource, int clear)
{
    text->allocation_size = text->width * 32 * text->height;
    BattleInterfaceLayer_QueueResource((BattleInterfaceLayer *)text, resource, text->horizontal_mode,
                                       text->vertical_mode, 0, clear);
    text->flags.bits.requested = 1;
    text->flags.bits.uploaded = 0;
}
extern "C" void BattleClippedText_Init(BattleClippedText *text, void *buffer, s8 width, s8 height,
                                       s8 horizontal_mode, s8 vertical_mode, s16 clip_left, s16 clip_right,
                                       u32 destination, u32 palette)
{
    func_0202cbd4(text, 0, sizeof(*text));
    text->width = width;
    text->height = height;
    text->horizontal_mode = horizontal_mode;
    text->vertical_mode = vertical_mode;
    text->clip_left = clip_left;
    text->clip_right = clip_right;
    text->destination = destination;
    text->palette = palette;
    GameText_Init(&text->text, (const u32 *const *)(gBattleContext + BATTLE_TEXT_FONT_TABLE_OFFSET), buffer, 0, 0, 0, 1, 1, 6, 6,
                  (u8)(text->width - 1), 0, 255, 0);
}
extern "C" void BattleTiledText_Init(BattleTiledText *text, void *buffer, s8 width, s8 height,
                                     s8 horizontal_mode, s8 vertical_mode, u32 destination, u32 palette)
{
    func_0202cbd4(text, 0, sizeof(*text));
    text->buffer = (u8 *)buffer;
    text->width = width;
    text->height = height;
    text->horizontal_mode = horizontal_mode;
    text->vertical_mode = vertical_mode;
    text->destination = destination;
    text->palette = palette;
    GameText_Init(&text->text, (const u32 *const *)(gBattleContext + BATTLE_TEXT_FONT_TABLE_OFFSET), buffer, 0, 0, 0, 1, 1, 6, 6,
                  (u8)(text->width - 1), 0, 255, 0);
}
