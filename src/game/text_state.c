#include <game/text.h>
#include <game/save_data.h>

void GameText_Init(GameText *text, const u32 *const *fonts, void *destination,
    const u8 *string, u8 x, u8 y, u8 spacing, u8 leading, u8 color,
    u8 space_width, u8 tile_pitch, int unused, u8 alignment_width, u8 margin)
{
    text->fonts = fonts;
    text->destination = destination;
    text->text = string;
    text->cursor.bits.x = x;
    text->cursor.bits.y = y;
    text->cursor.bits.spacing = spacing;
    text->line.bits.spacing = spacing;
    text->cursor.bits.leading = leading;
    text->cursor.bits.base_color = color;
    text->style.bits.color = color;
    text->style.bits.space_width = space_width;
    text->style.bits.tile_pitch = tile_pitch;
    text->margin = margin;
    if (alignment_width == 255)
        text->style.bits.alignment_width = tile_pitch;
    else
        text->style.bits.alignment_width = alignment_width;
    text->font.font = 0;
    text->scale_x = 4096;
    text->scale_y = 4096;
    text->style.bits.language = gSaveData[0x515];
    GameText_Reset(text);
}

void GameText_Reset(GameText *text)
{
    text->line.bits.height = 0;
    text->line.bits.alignment = 0;
    text->line.bits.measuring = 0;
    text->font.bits.alignment = 0;
    text->line.bits.whole_image = 0;
    text->font.bits.origin_x = text->cursor.bits.x;
    text->line.origin_y = text->cursor.bits.y;
    text->glyph.bits.x = 0;
    text->glyph.bits.y = 0;
    text->line.bits.start_x = 1023;
    text->bounds.origin.lines = 1;
    text->bounds.width = 0;
    text->bounds.height = 0;
    text->bounds.origin.bits.x = 1023;
    text->bounds.origin.bits.y = text->cursor.bits.y;
}
