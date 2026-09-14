#include <game/window_operations.h>

extern void func_0201b4cc(GameWindowManager *, GameWindow *, const GameWindowSkin *, u8 *, int);
extern void func_0201c65c(GameWindowManager *, GameWindow *, const GameWindowSkin *, u8 *);
extern void func_0202cbd4(void *, u8, u32);

void GameWindow_DrawSkin(GameWindowManager *manager, GameWindow *window) {
    unsigned skin_index = window->properties.shape.bits.skin;
    const GameWindowSkin *skin = data_0205671c[skin_index];
    u8 *buffer = !manager->state.bits.buffer_mode ? window->back : window->front;
    if (!skin_index) {
        func_0202cbd4(buffer, 0, window->state.bits.size);
        if (!manager->state.bits.buffer_mode)
            func_0202cbd4(window->front, 0, window->state.bits.size);
    } else if (!window->properties.shape.bits.position_mode &&
               ((window->properties.position.bits.x & 7) ||
                (window->properties.position.bits.y & 7))) {
        func_0201b4cc(manager, window, skin, buffer, 0);
        if (!manager->state.bits.buffer_mode)
            func_0201b4cc(manager, window, skin, window->front, 1);
    } else {
        func_0201c65c(manager, window, skin, buffer);
        if (!manager->state.bits.buffer_mode)
            func_0201c65c(manager, window, skin, window->front);
    }
}
