/*
 * Pause mode sprite (overlay 7, 0x0207E93C-0x0207EA4C).
 *
 * The sprite indicating the current mode, with a timed variant that hides itself
 * again.
 */

#include "pause_scene_internal.h"
#include <game/battle_scene.h>
#include <game/pause_mode_sprite.h>

extern "C" {
BattleModel *Overlay5ResourceA_Get(PauseModeSpriteTask *task);
void func_ov005_0206650c(void *task);
void func_ov005_02069084(void *model, int list);
}

extern "C" void PauseModeSprite_UpdateTimed(PauseModeSpriteTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (task->mode != (s8)WORK.unknown110) {
        func_ov005_0206650c(task);
        return;
    }
    /* Zero on entry leaves the sprite active until the scene mode changes. */
    if (task->counter) {
        --task->counter;
        if (!task->counter) {
            func_ov005_0206650c(task);
            return;
        }
    }
    int x = task->x / 4096, y = task->y / 4096;
    model->animation_offset_x = x;
    model->animation_offset_y = y;
    func_ov005_02069084(model, 5);
}

extern "C" void PauseModeSprite_Update(PauseModeSpriteTask *task)
{
    BattleModel *model = Overlay5ResourceA_Get(task);
    if (task->mode != (s8)WORK.unknown110) {
        func_ov005_0206650c(task);
        return;
    }
    int x = task->x / 4096, y = task->y / 4096;
    model->animation_offset_x = x;
    model->animation_offset_y = y;
    func_ov005_02069084(model, 5);
}
