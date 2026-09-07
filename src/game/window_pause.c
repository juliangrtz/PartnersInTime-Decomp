#include <game/window.h>
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
