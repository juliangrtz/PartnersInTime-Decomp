#include <game/window.h>
#include <game/save_data.h>
void GameWindow_Pause(GameWindowManager *manager, s16 index) {
    GameWindow *window = &manager->windows[index];
    int id = index << 8;
    window->text_state.bits.paused = 1;
    GameSpriteEffectPool_Pause(manager->effects, (u16)id, 0);
    GameSpriteEffectPool_Pause(manager->effects, (u16)(id + 255), 0);
    if (window->has_selection) GameSpriteEffectPool_Pause(manager->effects, (u16)(id + (window->selection + 1)), 0);
}
void GameWindow_Resume(GameWindowManager *manager, s16 index) {
    GameWindow *window = &manager->windows[index];
    int id = index << 8;
    window->text_state.bits.paused = 0;
    GameSpriteEffectPool_Resume(manager->effects, (u16)id);
    GameSpriteEffectPool_Resume(manager->effects, (u16)(id + 255));
    if (window->has_selection) GameSpriteEffectPool_Resume(manager->effects, (u16)(id + (window->selection + 1)));
}

u32 GameWindow_MeasureWindowSize(GameWindowManager *manager, const u8 *string, const u32 *const *fonts)
{
    GameTextBounds bounds;
    u16 width, height;
    GameWindow_MeasureText(manager, string, fonts, &bounds, gSaveData[0x515]);
    width = bounds.width + 24;
    if (width & 1) ++width;
    width = (width >> 3) - 1;
    height = bounds.height + 16;
    if (height & 1) ++height;
    height = (height >> 3) - 1;
    return width + (height << 16);
}
