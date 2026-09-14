#include <game/field_area.h>
#include <game/window.h>
struct FieldWindowCloseInterface {
    virtual void slot00();
    virtual void slot04();
    virtual void slot08();
    virtual void slot0c();
    virtual void slot10();
    virtual void close(s16 index);
};
static inline GameWindow *AreaWindow(FieldAreaContext *area, int index) {
    return &((GameWindowManager *)area->owner)->windows[(s16)index];
}

extern "C" void FieldArea_CloseMessageWindows(FieldAreaContext *area, int index) {
    if (index >= 0) {
        ((FieldWindowCloseInterface *)area->owner)->close((s16)index);
        return;
    }
    for (int i = 0; i < 8; ++i) {
        if (index != -2 && area->flags.screen != AreaWindow(area, i)->properties.shape.bits.screen)
            continue;
        ((FieldWindowCloseInterface *)area->owner)->close((s16)i);
    }
}
