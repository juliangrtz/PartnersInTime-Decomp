#include "pause_scene_internal.h"
#include <game/pause_hp_warning.h>
#include <game/battle_scene.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(PauseHpWarningTask *);
void func_ov005_0206650c(PauseHpWarningTask *);
void func_ov005_02069084(void *, int);
}

extern "C" void PauseHpWarning_Update(PauseHpWarningTask *task)
{
    BattleModel *sprite = Overlay5ResourceA_Get(task);
    if (WORK.hp_warning_mode == PAUSE_HP_WARNING_DISABLED) {
        func_ov005_0206650c(task);
        return;
    }
    if (WORK.hp_warning_mode == PAUSE_HP_WARNING_SUSPENDED)
        return;
    SavePartyMember *member = (SavePartyMember *)(gSaveData + 0x3f8) + task->member;
    if (100 * member->current_hp > 25 * member->max_hp) {
        sprite->animation_offset_x = -256;
        sprite->animation_offset_y = -256;
    } else {
        int y = task->y;
        sprite->animation_offset_x = task->x;
        sprite->animation_offset_y = y;
    }
    func_ov005_02069084(sprite, 60);
}
