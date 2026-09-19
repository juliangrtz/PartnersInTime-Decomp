#include "pause_scene_internal.h"
#include <game/overlay005_resource.h>
#include <game/pause_list_row.h>
#include <game/overlay007_party.h>
#include <game/scene_menu_motion.h>

extern "C" {
void *Overlay5ResourceB_Get(Overlay5ResourceOwner *owner);
void func_ov005_02069084(void *sprite, u8 list);
u8 func_ov007_02075400(Overlay7Party *party);
u8 func_ov007_02075324(Overlay7Party *, int);
u16 func_ov007_02075180(Overlay7Party *, int, int);
}

extern "C" void PauseListRow_Refresh(PauseListRowTask *task)
{
    int row = PauseList_GetTileRow((Overlay7Party *)data_ov007_0208e1e4);
    row = (row + task->row) % 9;
    task->x = task->origin_x + (data_ov007_020906f0.x << 12);
    task->y = task->origin_y + (data_ov007_020906f0.y << 12);
    task->tile = (34 * row + (WORK.main_allocation.offset >> 5)) >> 1;
    task->item = func_ov007_02075324((Overlay7Party *)data_ov007_0208e1e4, (u8)task->row);
    task->quantity = func_ov007_02075180((Overlay7Party *)data_ov007_0208e1e4, (u8)task->row, 0);
    if (PauseList_CheckRowAvailability((Overlay7Party *)data_ov007_0208e1e4, (u8)task->row, 4, 0) >= 0)
        task->palette_offset = 0;
    else
        task->palette_offset = 1;
}

extern "C" void PauseListRow_UpdateTextSprite(PauseListSpriteTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    PauseListRowTask *parent = task->parent;
    sprite->attributes_1_bits.tile = parent->tile + (8 * task->selection.segment) / 2;
    sprite->attributes_1_bits.palette =
        (data_ov005_0206a180.palettes[0].bank + parent->palette_offset) & 15;
    sprite->x = parent->x;
    sprite->y = parent->y;
    func_ov005_02069084(sprite, parent->draw_list);
}

extern "C" void PauseListRow_UpdateMarkerSprite(PauseListSpriteTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    PauseListRowTask *parent = task->parent;
    if ((func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 2 &&
         func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 3) || parent->item) {
        sprite->attributes_1_bits.tile = parent->tile + 16;
        sprite->x = parent->x;
        sprite->y = parent->y;
        func_ov005_02069084(sprite, parent->draw_list);
    }
}

extern "C" void PauseListRow_UpdateDigitSprite(PauseListSpriteTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    PauseListRowTask *parent = task->parent;
    if ((func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 2 &&
         func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 3) || parent->item) {
        int divisor = task->selection.divisor;
        u8 digit = (parent->quantity % (10 * divisor)) / divisor;
        if (digit || divisor == 1) {
            sprite->attributes_1_bits.tile = digit;
            sprite->attributes_1_bits.palette =
                (data_ov005_0206a180.palettes[0].bank + parent->palette_offset) & 15;
            sprite->x = parent->x;
            sprite->y = parent->y;
            func_ov005_02069084(sprite, parent->draw_list);
        }
    }
}
