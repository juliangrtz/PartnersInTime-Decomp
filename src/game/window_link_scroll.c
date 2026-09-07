#include <game/window.h>
void GameWindow_ScrollLink(GameWindowManager *manager, GameWindowLink *link) {
    s16 dx, dy;
    if (link->state.bits.linked) {
        if (link->state.bits.fresh) { link->state.bits.fresh = 0; return; }
        dx = 0;
        dy = 0;
        if (!link->state.bits.screen) {
            if (manager->main_origin_x != manager->main_scroll_x) dx = manager->main_scroll_x - manager->main_origin_x;
            if (manager->main_origin_y != manager->main_scroll_y) dy = manager->main_scroll_y - manager->main_origin_y;
        } else {
            if (manager->sub_origin_x != manager->sub_scroll_x) dx = manager->sub_scroll_x - manager->sub_origin_x;
            if (manager->sub_origin_y != manager->sub_scroll_y) dy = manager->sub_scroll_y - manager->sub_origin_y;
        }
        link->state.bits.x += dx;
        link->state.bits.y += dy;
    }
}
