#include "scene_menu_motion_internal.h"

extern "C" {

void SceneMenu_ApplyScroll(SceneMenuTweenTask *task)
{
    Overlay5DisplayBg_SetOffset(DISPLAY_ENGINE_MAIN, 1, -data_ov007_020906f0.x, -data_ov007_020906f0.y);
    Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_MAIN, 2, 4096, 4096, 0, 0, 0, -data_ov007_020906f0.x,
                                -data_ov007_020906f0.y);
    if (data_ov007_020906f0.x == task->target_x) {
        data_ov007_020906f0.y = task->target_y;
        if (data_ov007_020906f0.y)
            func_ov005_0206650c(task);
    }
}

void SceneMenu_UpdateScroll(SceneMenuTweenTask *task)
{
    if (task->elapsed >= task->duration) {
        func_ov005_0206650c(task);
        return;
    }
    task->velocity_x += task->acceleration_x;
    task->velocity_y += task->acceleration_y;
    task->x += task->velocity_x;
    task->y += task->velocity_y;
    ++task->elapsed;
    if (task->elapsed >= task->duration) {
        task->x = task->target_x;
        task->y = task->target_y;
    }
    data_ov007_020906f0.x = task->x / 16;
    data_ov007_020906f0.y = task->y / 16;
}

void SceneMenu_StartScroll(int x, int y, int duration)
{
    SceneMenuTweenTask *task =
        (SceneMenuTweenTask *)func_ov005_0206659c((void *)SceneMenu_ApplyScroll, 11, 1);
    task->target_x = x;
    task->target_y = y;
    task = (SceneMenuTweenTask *)func_ov005_0206659c((void *)SceneMenu_UpdateScroll, 1, 1);
    task->duration = duration;
    task->elapsed = 0;
    task->x = 16 * data_ov007_020906f0.x;
    task->y = 16 * data_ov007_020906f0.y;
    task->target_x = 16 * x;
    task->target_y = 16 * y;
    task->velocity_x = 0;
    task->velocity_y = 0;
    task->acceleration_x =
        _s32_div_f(2 * (task->target_x - task->x - task->velocity_x * duration), duration * duration);
    task->acceleration_y =
        _s32_div_f(2 * (task->target_y - task->y - task->velocity_y * duration), duration * duration);
}
}
