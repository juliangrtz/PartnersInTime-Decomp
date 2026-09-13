#include "pause_scene_internal.h"
#include <game/overlay005_resource.h>
#include <game/pause_list_row.h>

struct Overlay7Party;
extern "C" {
void *Overlay5ResourceB_Get(Overlay5ResourceOwner *owner);
void func_ov005_02069084(void *sprite, u8 list);
u8 func_ov007_02075400(Overlay7Party *party);
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
