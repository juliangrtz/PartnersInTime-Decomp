extern "C" {
#include <game/text.h>
#include <game/sprite_image.h>

static inline const u32 *GameText_FontForCode(const GameText *text, u16 code)
{
    int index = text->font.font;
    const u32 *const *fonts = text->fonts;
    return code < 0x100 ? fonts[index * 2] : fonts[index * 2 + 1];
}

static inline const u32 *GameText_GlyphImage(const u32 *font, u16 character)
{
    return font + (font[(u8)character + 2] >> 2);
}

/* Measurement returns packed dimensions; drawing returns the size aggregate. */
static inline u16 GameText_GlyphSize(s32 scale_x, s32 scale_y,
    u8 index, const u32 *image, int whole)
{
    GameSpriteEffectSize size;
    size.bytes.width = whole ? 4 * ((image[0] & 0xf0) >> 4)
        : GameSpriteImage_Width(image, index);
    size.bytes.height = 4 * (image[0] & 15) - 1;
    if (scale_x && scale_y && (scale_x != 4096 || scale_y != 4096))
        GameSpriteEffect_ScaleSize(scale_x, scale_y, &size);
    return size.raw;
}

GameTextToken GameText_Next(GameText *text, int measure, int skip_controls)
{
    GameTextToken token = { 0 };
    if (*text->text) {
        switch (*text->text) {
        case 0x20:
        {
            u8 width = text->style.bits.space_width - 1;
            u8 height;
            const u32 *font;
            text->glyph.bits.x = text->cursor.bits.x;
            text->glyph.bits.y = text->cursor.bits.y;
            if (text->line.bits.start_x == 1023)
                text->line.bits.start_x = text->cursor.bits.x;
            text->cursor.bits.x += width + 1 + text->line.bits.spacing;
            text->glyph.bits.width = width;
            ++text->text;
            token.value.fields.character.raw = 0xff20;
            token.value.fields.code = 0xff20;
            font = text->fonts[text->font.font * 2];
            height = 4 * (font[font[2] >> 2] & 15);
            if (height > text->line.bits.height)
                text->line.bits.height = height;
            break;
        }
        case 0xff:
            text->text += GameText_ProcessControl(text, &token, skip_controls) + 2;
            token.value.fields.code = token.value.fields.character.raw;
            break;
        default:
        {
            u16 prefix;
            u16 code;
            const u32 *font;
            int font_index;
            const u32 *const *fonts;
            u16 character;
            GameSpriteEffectSize size;
            u8 height;
            u32 width;
            token.text = text->text;
            text->glyph.bits.y = text->cursor.bits.y;
            if (text->style.bits.language)
                prefix = 0;
            else {
                prefix = 255 - *text->text;
                if (prefix < 6)
                    ++text->text;
                else
                    prefix = 0;
            }
            code = *text->text | (prefix << 8);
            font_index = text->font.font;
            fonts = text->fonts;
            if (code < 0x100)
                font = fonts[font_index * 2];
            else {
                font = fonts[font_index * 2 + 1];
                code -= 0x100;
            }
            token.value.fields.character.raw = ((const u16 *)(font + (font[1] >> 2)))[code];
            token.value.fields.code = (prefix << 8) | *text->text;
            character = token.value.fields.character.raw;
            if (!skip_controls && !measure) {
                int whole = text->line.bits.whole_image != 0;
                GameSpriteEffectSize drawn = GameSpriteImage_Draw(
                    text->cursor.bits.x, text->cursor.bits.y,
                    text->scale_x, text->scale_y,
                    (character >> 8) & 255,
                    text->style.bits.tile_pitch, text->destination,
                    GameText_GlyphImage(GameText_FontForCode(text, token.value.fields.code),
                        character & 255),
                    text->style.bits.color, whole);
                size = drawn;
            } else {
                GameSpriteEffectSize measured = { GameText_GlyphSize(
                    text->scale_x, text->scale_y,
                    (character >> 8) & 255,
                    GameText_GlyphImage(GameText_FontForCode(text, token.value.fields.code),
                        character & 255),
                    text->line.bits.whole_image ? 1 : 0) };
                size = measured;
            }
            text->glyph.bits.x = text->cursor.bits.x;
            text->glyph.bits.y = text->cursor.bits.y;
            if (text->line.bits.start_x == 1023)
                text->line.bits.start_x = text->cursor.bits.x;
            text->cursor.bits.x += size.bytes.width + 1 + text->line.bits.spacing;
            text->glyph.bits.width = size.bytes.width;
            text->glyph.bits.height = size.bytes.height;
            size.raw += 0x10001;
            height = size.bytes.height;
            if (height > text->line.bits.height)
                text->line.bits.height = height;
            ++text->text;
            if (text->bounds.origin.bits.x > text->line.bits.start_x)
                text->bounds.origin.bits.x = text->line.bits.start_x;
            width = text->cursor.bits.x - text->bounds.origin.bits.x;
            if (text->bounds.width < width)
                text->bounds.width = width;
            text->bounds.height = text->cursor.bits.y - text->bounds.origin.bits.y
                + text->line.bits.height;
            break;
        }
        }
    }
    return token;
}

}
