#include <game/window.h>

class GameWindowClearInterface {
public:
    virtual void reserved00();
    virtual void reserved04();
    virtual void reserved08();
    virtual void reserved0c();
    virtual void reserved10();
    virtual void reserved14();
    virtual void clear(int index);
};

extern "C" {
extern void func_020197ec(GameWindowManager *, GameWindow *);

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

void GameWindow_RedrawAfterEffect(GameSpriteEffect *effect)
{
    GameWindowManager *manager = (GameWindowManager *)effect->unknown10;
    int index = (u8)(effect->state.words.id >> 8);
    if ((u8)manager->windows[(s16)index].allocation.bits.suppress_redraw) return;
    ((GameWindowClearInterface *)manager)->clear(index);
    func_020197ec(manager, &manager->windows[index]);
    manager->windows[(s16)index].allocation.bits.dirty = 1;
    manager->state.raw |= 8;
}
}
