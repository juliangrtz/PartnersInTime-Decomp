/*
 * Equipment category cursor (overlay 7, 0x0207B1B0-0x0207B2C8).
 *
 * The cursor over the equipment category tabs.
 */

#include "pause_scene_internal.h"
#include <game/pause_selection_sprites.h>
#include <game/battle_scene.h>
#include <game/scene_menu_motion.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(void *);
BattleModel *Overlay5ResourceA_Attach(void *, BattleModel *, int);
void *func_ov005_020698dc(int);
void func_ov005_02068908(BattleModel *, int, void *, int, int);
void *func_ov005_02069084(void *, int);
extern u8 data_ov007_02090717[];
}

extern "C" void PauseEquipment_UpdateCategoryCursor(PauseSelectionSpriteTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    int y = task->y + 24 * data_ov007_02090717[WORK.selected_menu];
    sprite->animation_offset_x = task->x;
    sprite->animation_offset_y = y;
    func_ov005_02069084(sprite, 8);
}

extern "C" void PauseEquipment_CreateCategoryCursor(void)
{
    void *resource = func_ov005_020698dc(75);
    PauseSelectionSpriteTask *task = (PauseSelectionSpriteTask *)func_ov005_0206659c(
        (void (*)(PauseMenuElement *))PauseEquipment_UpdateCategoryCursor, 3, 1);
    BattleModel *sprite = Overlay5ResourceA_Attach(task, 0, 1);
    func_ov005_02068908(sprite, 0, resource, 1, -1);
    sprite->set_primary_animation(0, 0, 1);
    sprite->flags = (sprite->flags & ~3) | 1;
    task->x = data_ov007_020906f0.x + 60;
    if (WORK.special_available)
        task->y = data_ov007_020906f0.y + 87;
    else
        task->y = data_ov007_020906f0.y + 95;
}
