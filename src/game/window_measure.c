#include <game/window.h>
void GameWindow_MeasureText(GameWindowManager *manager, const u8 *string, const u32 *const *fonts, GameTextBounds *bounds, u8 compact) {
    GameText text;
    GameText_Init(&text, fonts, 0, string, 0, 0, compact == 0 ? 2 : 1, compact == 0 ? 3 : 1, 1, 6, 64, compact, 255, 0);
    GameText_MeasureBounds(&text, bounds);
}
