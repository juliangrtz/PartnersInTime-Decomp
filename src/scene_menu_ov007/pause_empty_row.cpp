/*
 * Empty equipment row (overlay 7, 0x02073428-0x0207355C).
 *
 * The sprite drawn in place of a row when an equipment slot is empty.
 */

#include "pause_scene_internal.h"
#include <game/pause_list_row.h>
#include <game/battle_scene.h>
#include <game/overlay007_party.h>
#include <game/scene_menu_motion.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(PauseMenuElement *);
BattleModel *Overlay5ResourceA_Attach(PauseMenuElement *, BattleModel *, int);
void *func_ov005_020698dc(int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void *func_ov005_02069084(void *, int);
u8 func_ov007_020750bc(Overlay7Party *);
u8 func_ov007_02075324(Overlay7Party *, int);
}
#define PARTY ((Overlay7Party *)data_ov007_0208e1e4)

extern "C" void PauseList_UpdateEmptyRowSprite(PauseMenuElement *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    int rows = func_ov007_020750bc(PARTY);
    if (rows > 9) rows = 9;
    int row = 0;
    for (; row < rows; ++row) {
        if (!func_ov007_02075324(PARTY, (u8)row)) break;
    }
    if (row != rows) {
        s16 y = data_ov007_020906f0.y + 50 + 12 * row;
        sprite->animation_offset_x = data_ov007_020906f0.x + 70;
        sprite->animation_offset_y = y;
        func_ov005_02069084(sprite, 39);
    }
}

extern "C" void PauseList_CreateEmptyRowSprite(void)
{
    PauseMenuElement *task = func_ov005_0206659c(PauseList_UpdateEmptyRowSprite, 2, 1);
    BattleModel *sprite = Overlay5ResourceA_Attach(task, 0, 1);
    void *resource = func_ov005_020698dc(82);
    func_ov005_02068908(sprite, 0, resource, 0, -1);
    sprite->set_primary_animation(0, 0, 1);
    sprite->flags = (sprite->flags & ~3) | 2;
}
