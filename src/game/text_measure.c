/*
 * Text measurement (ARM9 resident, 0x02025DD8-0x02026000).
 *
 * Runs the text walk without drawing to get a line's width, a block's height or
 * its full bounds, and installs a control-code handler.
 */

#include <game/text.h>
#include <game/sprite_output.h>

void GameText_SetControl(int index, s8 length, GameTextCallback callback, void *argument)
{
    data_0205a934[index].length = length;
    data_0205a934[index].callback = callback;
    data_0205a934[index].argument = argument;
}

void GameText_MeasureBounds(const GameText *text, GameTextBounds *bounds)
{
    GameText copy;
    GameSprite_CopyBytes(text, &copy, sizeof(copy));
    while (*copy.text)
        GameText_Next(&copy, 1, 1);
    GameSprite_CopyBytes(&copy.bounds, bounds, sizeof(*bounds));
}

u16 GameText_MeasureHeight(const GameText *text)
{
    GameText copy;
    GameSprite_CopyBytes(text, &copy, sizeof(copy));
    copy.line.bits.measuring = 1;
    while (*copy.text)
        GameText_Next(&copy, 1, 1);
    return copy.bounds.height + (copy.bounds.origin.lines << 8);
}

u16 GameText_MeasureLine(const GameText *text)
{
    GameText copy;
    GameSprite_CopyBytes(text, &copy, sizeof(copy));
    copy.line.bits.measuring = 1;
    for (;;) {
        if (*copy.text == 255 && copy.text[1] == 0)
            break;
        if (*copy.text == 0)
            break;
        GameText_Next(&copy, 1, 1);
    }
    return copy.cursor.bits.x - text->cursor.bits.x;
}
