#include <game/window.h>
extern void func_0202cbd4(void *, u8, u32);
extern void func_0201b37c(GameWindowManager *, GameWindow *);
void GameWindow_RebuildTilemap(GameWindowManager *manager, int screen) {
    GameWindow *window;
    if (!screen) {
        func_0202cbd4(manager->main_tilemap, 0, 1920);
        window = manager->main_windows;
    } else {
        func_0202cbd4(manager->sub_tilemap, 0, 1920);
        window = manager->sub_windows;
    }
    for (; window->next != (GameWindow *)-1; window = window->next) {
        if (!window->properties.shape.bits.position_mode) func_0201b37c(manager, window);
    }
    if (screen) manager->state.bits.sub_map_dirty = 1;
    else manager->state.bits.main_map_dirty = 1;
}
