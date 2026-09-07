#include <game/text.h>

static inline void GameText_AlignLine(GameText *text)
{
    switch (text->line.bits.alignment) {
    case 1:
        text->cursor.bits.x = 4 * (text->style.bits.alignment_width + 1)
            - (GameText_MeasureLine(text) >> 1);
        text->cursor.bits.x += text->margin;
        break;
    case 2:
        text->cursor.bits.x = 8 * (text->style.bits.alignment_width + 1)
            - GameText_MeasureLine(text);
        text->cursor.bits.x -= text->font.bits.origin_x - text->margin;
        text->cursor.bits.x += text->margin;
        break;
    }
}

int GameText_ProcessControl(GameText *text, GameTextToken *token, int skip_controls)
{
    u32 code = text->text[1];
    int length = 0;
    if (code == 0) {
        ++text->bounds.origin.lines;
        text->cursor.bits.y += text->line.bits.height + text->cursor.bits.leading;
        text->cursor.bits.x = text->font.bits.origin_x;
        text->line.bits.height = 0;
        text->line.bits.start_x = 1023;
        if (!text->line.bits.measuring) {
            text->text += 2;
            GameText_AlignLine(text);
            text->text -= 2;
        }
        token->value.fields.character.raw = 0xff00;
    } else if (code < 0x20) {
        GameTextControl *control;
        GameTextCallback callback;
        code &= 0x1f;
        control = &data_0205a934[code];
        callback = control->callback;
        if (callback) {
            if (control->length < 0)
                length = callback(control->argument, text, skip_controls);
            else {
                callback(control->argument, text, skip_controls);
                length = control->length;
            }
        } else
            length = control->length;
        if (!skip_controls) {
            token->value.fields.character.raw = 0xff00 | code;
            token->text = text->text + 2;
        }
    } else if (code < 0x30 && !skip_controls) {
        text->style.bits.color = (code & 15) + text->cursor.bits.base_color;
        if (text->style.bits.color > 14)
            text->style.bits.color = 14;
    } else if (code < 0x40) {
        u8 mode = code & 15;
        if (mode < 4) {
            if ((mode & 3) == 0) {
                text->scale_x = 4096;
                text->scale_y = 4096;
            } else {
                if (mode & 1) text->scale_y = 2048;
                if (mode & 2) text->scale_x = 2048;
            }
        } else if (mode < 7) {
            if (!text->line.bits.measuring) {
                u8 alignment = mode - 4;
                if (text->font.bits.alignment != alignment) {
                    text->font.bits.alignment = alignment;
                    text->line.bits.alignment = alignment;
                    GameText_AlignLine(text);
                    token->value.fields.character.raw = 0xff30 + text->line.bits.alignment;
                    token->value.fields.code = 0xff30 + text->line.bits.alignment;
                }
            }
        } else if (mode < 9)
            text->line.bits.whole_image = (mode & 8) >> 3;
    } else if (code < 0x50) {
        text->font.font = code & 15;
    } else if (code < 0x60) {
        text->style.bits.space_width = code & 15;
    } else if (code < 0x80) {
        text->cursor.bits.x += code - 0x5f;
    } else if (code < 0xa0) {
        text->cursor.bits.x -= code - 0x7f;
    } else if (code < 0xc0) {
        text->cursor.bits.y += code - 0x9f;
    } else if (code < 0xe0) {
        text->cursor.bits.y -= code - 0xbf;
    } else if (code < 0xe8) {
        u8 spacing = code - 0xe0;
        text->cursor.bits.spacing = spacing;
        text->line.bits.spacing = spacing;
    } else if (code < 0xf0) {
        u8 spacing = code - 0xe8;
        if (spacing == 7)
            text->line.bits.spacing = text->cursor.bits.spacing;
        else
            text->line.bits.spacing = spacing;
    }
    return length;
}
