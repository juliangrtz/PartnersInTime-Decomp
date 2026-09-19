/*
 * Equipment member arrows (overlay 7, 0x0207F268-0x0207F388).
 *
 * The arrows that switch which party member the equipment screen is showing.
 */

#include "pause_scene_internal.h"
#include <game/battle_scene.h>
#include <game/menu_equipment.h>

extern "C" {
void func_ov005_0206650c(MenuEquipmentTask *);
BattleModel *Overlay5ResourceA_Get(MenuEquipmentTask *);
void func_ov005_02069084(void *, int);
extern s16 FX_SinCosTable_[];
}

extern "C" void MenuEquipment_UpdateMemberArrow(MenuEquipmentTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    if (!WORK.equipment_active) {
        func_ov005_0206650c(task);
        return;
    }
    if (task->member_arrow.direction == WORK.equipment_switching)
        sprite->set_primary_animation(7, 0, 1);
    else
        sprite->set_primary_animation(1, 0, 1);
    int x = WORK.equipment_heading_x + task->member_arrow.x;
    int y = WORK.equipment_heading_y + task->member_arrow.y;
    if (!WORK.equipment_switching) {
        task->member_arrow.angle += 1536;
        /* The accumulated angle wraps to 16 bits only for the sine lookup. */
        x += task->member_arrow.amplitude *
            FX_SinCosTable_[2 * ((task->member_arrow.angle & 0xffff) >> 4)];
    }
    sprite->animation_offset_x = x / 4096;
    sprite->animation_offset_y = y / 4096;
    func_ov005_02069084(sprite, 6);
}
