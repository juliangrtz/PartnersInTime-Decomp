#include <game/window.h>
extern void func_0201e79c(GameWindowManager *, GameWindow *);
extern void GameWindow_ScrollLink(GameWindowManager *, GameWindowLink *);
void GameWindow_Update(GameWindowManager *manager) {
    GameWindow *window;
    for (window = manager->main_windows; window->next != (GameWindow *)-1; window = window->next) {
        window->allocation.bits.suppress_redraw = 0;
        manager->display.bits.current_window = window->allocation.bits.group;
        if (window->text.text) func_0201e79c(manager, window);
        if (!window->properties.shape.bits.position_mode) {
            GameWindow_ScrollLink(manager, &window->links[0]);
            GameWindow_ScrollLink(manager, &window->links[1]);
        }
    }
    for (window = manager->sub_windows; window->next != (GameWindow *)-1; window = window->next) {
        window->allocation.bits.suppress_redraw = 0;
        manager->display.bits.current_window = window->allocation.bits.group;
        if (window->text.text) func_0201e79c(manager, window);
        if (!window->properties.shape.bits.position_mode) {
            GameWindow_ScrollLink(manager, &window->links[0]);
            GameWindow_ScrollLink(manager, &window->links[1]);
        }
    }
    GameSpriteEffectPool_Update(manager->effects);
    GameSpriteAnimator_Update(manager->animator);
    manager->main_scroll_x = manager->main_origin_x;
    manager->main_scroll_y = manager->main_origin_y;
    manager->sub_scroll_x = manager->sub_origin_x;
    manager->sub_scroll_y = manager->sub_origin_y;
}
