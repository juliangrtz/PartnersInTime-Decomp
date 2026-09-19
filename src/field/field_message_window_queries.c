/*
 * Message window queries (overlay 0, 0x0206FB74-0x0206FD18).
 *
 * Whether the area has a message window open, and whether one is open but not
 * already closing - which is what a script checks before opening another.
 */

#include <game/field_area.h>
#include <game/window.h>
static inline GameWindow *AreaWindow(FieldAreaContext *area, int index) {
    return &((GameWindowManager *)area->owner)->windows[(s16)index];
}
static inline u8 WindowOpen(FieldAreaContext *area, int i) {
    return AreaWindow(area, i)->front != 0;
}
static inline u8 WindowClosing(FieldAreaContext *area, int i) {
    return AreaWindow(area, i)->allocation.bits.closing;
}

int FieldArea_HasNonClosingWindow(FieldAreaContext *area, int window) {
    int i;
    if (window >= 0)
        return (WindowClosing(area, window) ^ 1) != 0;
    for (i = 0; i < 8; ++i) {
        if ((window == -2 ||
             area->flags.screen == AreaWindow(area, i)->properties.shape.bits.screen) &&
            !WindowClosing(area, i))
            return 1;
    }
    return 0;
}

int FieldArea_HasOpenMessageWindow(FieldAreaContext *area, int window) {
    int i;
    if (window >= 0)
        return WindowOpen(area, window) != 0;
    for (i = 0; i < 8; ++i) {
        if ((window == -2 ||
             area->flags.screen == AreaWindow(area, i)->properties.shape.bits.screen) &&
            WindowOpen(area, i))
            return 1;
    }
    return 0;
}
