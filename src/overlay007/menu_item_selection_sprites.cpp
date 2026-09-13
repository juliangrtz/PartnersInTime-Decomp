#include "pause_scene_internal.h"
#include <game/menu_item_selection.h>
#include <game/menu_item_effect.h>

/* Interior alias of the first two display palettes' bank bytes, 20 bytes apart. */
struct SelectionPaletteEntry { u8 bank, unknown_01[19]; };
typedef char SelectionPaletteStride[sizeof(SelectionPaletteEntry) == sizeof(GameSpritePalette) ? 1 : -1];
extern "C" {
extern SelectionPaletteEntry data_ov005_0206a1f1[2];
MenuItemEffectSprite *Overlay5ResourceB_Get(MenuItemSelectionTask *);
void func_ov005_0206650c(void *);
void func_ov005_02069084(void *, int);
}

/* Preserve the raw workspace view: selection fields at WORK + 0x12D/+0x12E. */
static inline u8 SelectionBlend() { return WORK.unknown127[6]; }
static inline u8 SelectionState() { return WORK.unknown127[7]; }

extern "C" void MenuItemSelection_UpdateLabelStrip(MenuItemSelectionTask *task)
{
    MenuItemSelectionOwner *parent = task->parent;
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    if (SelectionState() > 1) {
        func_ov005_0206650c(task);
        return;
    }
    sprite->x = (parent->x + 16 * parent->offset_x) / 4096;
    sprite->y = (parent->y + 16 * parent->offset_y) / 4096;
    sprite->x -= parent->half_width - task->offset_x;
    sprite->y -= task->offset_y;
    if (task->sub_screen)
        sprite->y += 240;
    sprite->x <<= 12;
    sprite->y <<= 12;
    if (SelectionBlend())
        sprite->attributes_0 = (sprite->attributes_0 & ~0xc00) | 0x400;
    else
        sprite->attributes_0 &= ~0xc00;
    if (parent->status >= 0)
        sprite->attributes_1_bits.palette = data_ov005_0206a1f1[sprite->screen].bank & 15;
    else
        sprite->attributes_1_bits.palette = (data_ov005_0206a1f1[sprite->screen].bank + 1) & 15;
    if (!SelectionState())
        func_ov005_02069084(sprite, 5);
    else
        func_ov005_02069084(sprite, 59);
}

extern "C" void MenuItemSelection_UpdateOffsetSprite(MenuItemSelectionTask *task)
{
    MenuItemSelectionOwner *parent = task->parent;
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    sprite->x = (parent->x + 16 * parent->offset_x) / 4096;
    sprite->y = (parent->y + 16 * parent->offset_y) / 4096;
    sprite->x -= 4;
    sprite->y -= 24;
    if (task->sub_screen)
        sprite->y += 240;
    sprite->x <<= 12;
    sprite->y <<= 12;
    if (SelectionBlend())
        sprite->attributes_0 = (sprite->attributes_0 & ~0xc00) | 0x400;
    else
        sprite->attributes_0 &= ~0xc00;
    func_ov005_02069084(sprite, 5);
}
