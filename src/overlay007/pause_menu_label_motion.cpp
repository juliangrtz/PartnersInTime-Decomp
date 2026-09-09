#include "pause_scene_internal.h"
#include <game/battle_scene.h>
extern "C" {
#include <game/pause_menu_label.h>
#include <game/menu_item_effect.h>
MenuItemEffectSprite *Overlay5ResourceB_Get(PauseMenuLabelTask *);
BattleModel *Overlay5ResourceA_Get(PauseMenuLabelTask *);
void func_ov005_02069084(void *, int);
}
static inline void SetPosition(BattleModel *sprite, int x, int y)
{
    sprite->animation_offset_x = x;
    sprite->animation_offset_y = y;
}
extern "C" void PauseMenuLabel_UpdateIconRow(PauseMenuLabelTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    SetPosition(sprite, WORK.label_origin_x + task->x, WORK.label_origin_y + task->y);
    func_ov005_02069084(sprite, 40);
}
extern "C" void PauseMenuLabel_UpdateTextRow(PauseMenuLabelTask *task)
{
    MenuItemEffectSprite *sprite = Overlay5ResourceB_Get(task);
    sprite->x = (WORK.label_origin_x + task->x) << 12;
    sprite->y = (WORK.label_origin_y + task->y) << 12;
    func_ov005_02069084(sprite, 40);
}