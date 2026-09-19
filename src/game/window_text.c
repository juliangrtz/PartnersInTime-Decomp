/*
 * Window text layout (ARM9 resident, 0x0201DA48-0x0201DC98).
 *
 * Vertical alignment of a window's text within its frame, and the sound played
 * when text starts appearing.
 */

#include <game/window.h>
#include <game/audio.h>
void GameWindow_StartTextSound(GameWindowManager *manager, GameWindow *window) {
    if (window->properties.shape.bits.sound) {
        GameAudio_StopEffect((s16)window->properties.shape.bits.sound);
        GameAudio_PlayEffectDelayed((s16)(window->properties.shape.bits.sound + 1), 0, -1);
        window->state.bits.animation = 3;
    }
}

void GameWindow_AlignText(GameWindowManager *manager, GameWindow *window, GameText *text, int alignment) {
    u16 height;
    if (!window->measured_height) window->measured_height = GameText_MeasureHeight(text);
    height = window->properties.shape.bits.height;
    switch (alignment) {
    case 0: text->cursor.bits.y = window->text_state.bytes.origin_y; break;
    case 1:
        text->cursor.bits.y = (8 * (height + 1) - (window->measured_height & 255)) >> 1;
        if (!window->properties.shape.bits.position_mode) text->cursor.bits.y += window->properties.position.bits.y & 7;
        break;
    case 2:
        text->cursor.bits.y = 8 * (height + 2 - data_0205671c[window->properties.shape.bits.skin]->border_y);
        text->cursor.bits.y -= (window->measured_height & 255) + text->cursor.bits.leading * (window->measured_height >> 8) + 1;
        if (!window->properties.shape.bits.position_mode) text->cursor.bits.y += window->properties.position.bits.y & 7;
        break;
    }
    if (!window->state.bits.layout_mode) text->bounds.origin.bits.y = text->cursor.bits.y;
}
