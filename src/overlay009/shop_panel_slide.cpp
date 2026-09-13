#include "shop_panel_slide_internal.h"

extern "C" void ShopPanelSlide_Update(ShopPanelSlideTask *task)
{
    task->current += task->velocity;
    task->velocity += task->acceleration;
    s32 target = task->target;
    if (!target) {
        if (task->current >= 0) task->current = target;
    } else {
        if (task->current <= -327680) task->current = target;
    }
    data_ov009_0207ea3c.panel_offset_y = task->current;
    if (!--task->frames) func_ov005_0206650c(task);
}

extern "C" void ShopPanelSlide_Begin(int entering)
{
    ShopPanelSlideTask *task = (ShopPanelSlideTask *)func_ov005_0206659c(
        (void (*)(ShopMenuElement *))ShopPanelSlide_Update, 0, 1);
    if (!entering) {
        task->current = 0;
        task->target = -458752;
    } else {
        task->current = -458752;
        task->target = 0;
    }
    task->velocity = 2 * (task->target - task->current) / 20;
    if (!entering) {
        task->acceleration = task->velocity / 20;
        task->velocity = 0;
    } else {
        task->acceleration = -task->velocity / 20;
    }
    task->frames = 20;
    data_ov009_0207ea3c.panel_offset_y = task->current;
}
