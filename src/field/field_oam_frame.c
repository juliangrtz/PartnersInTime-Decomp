#include <game/field_area.h>
#include <game/field_timer.h>
#include <game/field_hud.h>
#include <game/palette_animation.h>
extern void func_02009254(int, int, int, int);
extern void func_02009598(GameOamEntry *, int, int);
extern GameOamEntry data_02060340[2][128];
extern GameOamEntry *func_ov000_020be950(FieldTimer *, GameOamEntry *);
typedef struct FieldPaletteView {
    u8 prefix[9524];
    GamePaletteEffectController *palettes;
} FieldPaletteView;
typedef struct PaletteFrameVTable {
    void (*unknown0)(void *);
    void (*unknown4)(void *);
    void (*update)(GamePaletteEffectController *);
} PaletteFrameVTable;
GameOamEntry *FieldArea_PrepareOamFrame(FieldAreaContext *area, GameOamEntry *output) {
    if (area->unknown_23f0 == 71 && area->unknown_23f2 == 71) {
        FieldTimer *timer;
        int screen;
        func_02009254(area->flags.screen, data_0205a05c[area->flags.screen],
                      data_0205a060[area->flags.screen], 0);
        func_02009598(data_02060340[area->flags.screen], 0, data_0205a060[area->flags.screen]);
        timer = area->timer;
        screen = area->flags.screen;
        output += data_0205a05c[screen];
        if (timer && timer->flags.screen == screen)
            output = func_ov000_020be950(timer, output);
        if (area->flags.screen == 1)
            output = FieldHud_Draw(area, output);
        ((PaletteFrameVTable *)((FieldPaletteView *)area)->palettes->vtable)
            ->update(((FieldPaletteView *)area)->palettes);
    }
    return output;
}
