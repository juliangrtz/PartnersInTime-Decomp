#include <game/window_operations.h>
void GameWindow_ReleaseAll(GameWindowManager *manager) {
    s16 i;
    if (manager->windows) {
        for (i = manager->window_capacity - 1; i >= 0; --i) GameWindow_Release(manager, i);
        GameHeap_DeleteArray(manager->windows);
        manager->windows = 0;
        manager->window_capacity = 0;
    }
}
void GameWindow_SetOrigin(GameWindowManager *manager, int screen, s16 x, s16 y) {
    if (!screen) { manager->main_origin_x = x; manager->main_origin_y = y; }
    else { manager->sub_origin_x = x; manager->sub_origin_y = y; }
}
void GameWindow_Clear(GameWindowManager *manager, s16 index) {
    GameWindow_DrawSkin(manager, &manager->windows[index]);
    manager->windows[index].allocation.bits.suppress_redraw = 1;
}
