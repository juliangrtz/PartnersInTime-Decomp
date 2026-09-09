#include "pause_scene_internal.h"
#include <game/battle_scene.h>
#include <game/pause_menu_cursor.h>
/* The menu renderer uses this halfword as its palette-bank selector. */
struct MenuSpritePaletteView {
    u8 unknown_00[96];
    u16 palette_bank;
    u8 unknown_62[2];
};
typedef char MenuSpritePaletteView_SizeCheck[sizeof(MenuSpritePaletteView) == 100 ? 1 : -1];
/* Dispatch-only view of the palette controller's existing virtual interface. */
struct MenuPaletteDispatch {
    virtual void unknown_00();
    virtual void unknown_04();
    virtual void unknown_08();
    virtual void unknown_0c();
    virtual void configure(int index, u32 palette_mask, s16 mode, u16 color, int duration, u8 repeat,
                           s16 speed_q8, int elapsed_q8, u8 paused);
};
typedef char MenuPaletteDispatch_SizeCheck[sizeof(MenuPaletteDispatch) == 4 ? 1 : -1];
extern "C" {
BattleModel *Overlay5ResourceA_Get(PauseMenuCursorTask *);
BattleModel *Overlay5ResourceA_Attach(PauseMenuCursorTask *, BattleModel *, int);
void *func_ov005_020698dc(int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void func_ov005_02069084(void *, int);
extern u8 data_ov007_0208db38[], data_ov007_0208db39[];
}
static inline void Pulse(BattleModel *sprite, int mode)
{
    ((MenuPaletteDispatch *)WORK.palette_controller)
        ->configure(0, 1 << ((MenuSpritePaletteView *)sprite)->palette_bank, mode, 0x4210, 24, 0, 256, 0, 0);
}
extern "C" void PauseMenuCursor_UpdateShadow(PauseMenuCursorTask *task)
{
    PauseMenuCursorTask *parent = task->parent;
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    if ((++task->counter & 1) == 0) {
        s16 x = parent->x / 4096, y = parent->y / 4096;
        sprite->animation_offset_x = x;
        sprite->animation_offset_y = y;
        func_ov005_02069084(sprite, 41);
    }
}
extern "C" void PauseMenuCursor_UpdateIcon(PauseMenuCursorTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    void *resource = func_ov005_020698dc(83);
    if (!task->state) {
        if (task->page != WORK.highlighted_menu)
            return;
        GamePaletteEffects_ResetEntry(WORK.palette_controller, 0);
        sprite = Overlay5ResourceA_Attach(task, 0, 1);
        func_ov005_02068908(sprite, 0, resource, 0, -1);
        sprite->set_primary_animation(task->page, 0, 1);
        sprite->flags |= 8;
        sprite->flags = (sprite->flags & ~3) | 1;
        task->x = data_ov007_0208db38[2 * task->page] << 12;
        task->y = (data_ov007_0208db39[2 * task->page] + 16) << 12;
        task->icon.scale_x = 4096;
        task->icon.scale_y = 4096;
        if (task->immediate) {
            task->immediate = 0;
            task->counter = 0;
            task->state = PAUSE_MENU_ICON_PULSE_REVERSE;
        } else
            task->state = PAUSE_MENU_ICON_GROW_Y;
    }
    if (task->page != WORK.highlighted_menu) {
        if (sprite)
            func_ov005_02068c54(sprite);
        task->resource = 0;
        task->state = 0;
        return;
    }
    switch (task->state) {
    case PAUSE_MENU_ICON_GROW_Y:
        task->icon.scale_y += 512;
        if (task->icon.scale_y >= 6144) {
            task->icon.scale_y = 6144;
            task->counter = 0;
            ++task->state;
        }
        break;
    case PAUSE_MENU_ICON_SHRINK_Y:
        task->icon.scale_y -= 512;
        task->icon.scale_x += 409;
        if (task->icon.scale_y <= 4096) {
            task->icon.scale_y = 4096;
            ++task->state;
        }
        break;
    case PAUSE_MENU_ICON_GROW_X:
        task->icon.scale_x += 409;
        if (task->icon.scale_x >= 4915) {
            task->icon.scale_x = 4915;
            ++task->state;
        }
        break;
    case PAUSE_MENU_ICON_SHRINK_X:
        task->icon.scale_x -= 409;
        if (task->icon.scale_x <= 4096) {
            task->icon.scale_x = 4096;
            task->counter = 16;
            task->state = PAUSE_MENU_ICON_PULSE_REVERSE;
        }
        break;
    case PAUSE_MENU_ICON_PULSE_REVERSE:
        if (task->counter)
            --task->counter;
        else if (GamePaletteEffects_IsPaused(WORK.palette_controller, 0)) {
            Pulse(sprite, -5);
            ++task->state;
        }
        break;
    case PAUSE_MENU_ICON_PULSE_FORWARD:
        if (GamePaletteEffects_IsPaused(WORK.palette_controller, 0)) {
            Pulse(sprite, 5);
            --task->state;
        }
        break;
    }
    sprite = Overlay5ResourceA_Get(task);
    int y = task->y - 16 * task->icon.scale_y;
    sprite->animation_offset_x = task->x / 4096;
    sprite->animation_offset_y = y / 4096;
    s16 scale_x = task->icon.scale_x / 16, scale_y = task->icon.scale_y / 16;
    sprite->scale_x = scale_x;
    sprite->scale_y = scale_y;
    func_ov005_02069084(sprite, 60);
}
