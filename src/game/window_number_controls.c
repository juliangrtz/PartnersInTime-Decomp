#include <game/window.h>

extern void func_0201a5fc(GameWindowManager *, GameWindow *, u8, int);
extern void func_02019e74(GameWindowManager *, GameText *, GameWindowNumber *);

int GameWindow_MeasureNumber(void *argument, GameText *text, int process)
{
    GameWindowManager *manager = argument;
    GameWindow *window;
    GameWindowNumber *number;
    if (!process) return 0;
    window = &manager->windows[manager->display.bits.current_window];
    number = &window->number;
    if (!number->ready)
        func_0201a5fc(manager, window, (u8)(text->text[2] - 1), 0);
    func_02019e74(manager, text, number);
    return 0;
}

int GameWindow_MeasurePaddedNumber(void *argument, GameText *text, int process)
{
    GameWindowManager *manager = argument;
    GameWindow *window;
    GameWindowNumber *number;
    if (!process) return 0;
    window = &manager->windows[manager->display.bits.current_window];
    number = &window->number;
    if (!number->ready)
        func_0201a5fc(manager, window, (u8)(text->text[2] - 1), 1);
    func_02019e74(manager, text, number);
    return 0;
}
