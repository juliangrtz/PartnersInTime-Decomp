#include "shop_list_effects_internal.h"

extern "C" void ShopListScale_Update(ShopListScaleTask *task)
{
    if (task->delay) {
        --task->delay;
        if (!task->delay)
            func_ov005_02069bcc(234, 0, 0, 128);
    } else {
        task->current += task->step;
        if (!--task->frames)
            task->current = task->target;
        ShopList_SetScale(data_ov009_0207ea2c, task->current);
        if (!task->frames)
            func_ov005_0206650c(task);
    }
}

extern "C" void ShopList_AnimateScale(s32 initial, s32 target, int frames, int delay)
{
    ShopListScaleTask *task = (ShopListScaleTask *)func_ov005_0206659c(
        (void (*)(ShopRowTask *))ShopListScale_Update, 6, 1);
    task->frames = frames;
    task->current = initial;
    task->target = target;
    task->step = (task->target - task->current) / frames;
    task->delay = delay;
    ShopList_SetScale(data_ov009_0207ea2c, task->current);
}
