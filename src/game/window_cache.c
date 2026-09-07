#include <game/window.h>
void GameWindow_ResetTextCache(GameWindowManager *manager, GameWindow *window) {
    GameWindowTextCache *cache = &manager->text_cache[window->allocation.bits.group];
    cache->cursor = cache->commands;
    cache->x = window->text_state.bytes.origin_x;
    cache->y = window->text_state.bytes.origin_y;
    cache->scale_x = 4096;
    cache->scale_y = 4096;
    cache->color = window->text.cursor.bits.base_color;
    cache->font = window->text.font.font;
    cache->cursor[0] = 255;
    cache->cursor[1] = 255;
}
