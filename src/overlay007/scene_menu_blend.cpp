#include "scene_menu_motion_internal.h"

extern "C" {

void SceneMenu_UpdateZoom(SceneMenuTweenTask *task)
{
    Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_MAIN, 2, 16 * task->scale, 16 * task->scale, 0, 0, 0,
                                -task->x / 16, -task->y / 16);
    if (task->elapsed >= task->duration) {
        if (task->parent->pending)
            --task->parent->pending;
        func_ov005_0206650c(task);
    } else {
        task->scale_velocity += task->scale_acceleration;
        task->scale += task->scale_velocity;
        task->velocity_x += task->acceleration_x;
        task->velocity_y += task->acceleration_y;
        task->x += task->velocity_x;
        task->y += task->velocity_y;
        ++task->elapsed;
        if (task->elapsed >= task->duration) {
            task->scale = task->target_scale;
            task->x = task->target_x;
            task->y = task->target_y;
        }
    }
}

void SceneMenu_UpdateBlend(SceneMenuBlendTask *task)
{
    switch (task->phase) {
    case 0:
        task->blend += task->blend_step;
        if (task->blend >= 0x10000) {
            task->blend = 0x10000;
            ++task->phase;
        }
        *(vu16 *)0x4000052 = (task->blend / 4096) | ((16 - task->blend / 4096) << 8);
        break;
    case 1: {
        u16 palette;
        u16 *tiles;
        int i;
        if (!task->use_alternate_characters)
            Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 3, 8);
        else
            Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 3, 12);
        palette = task->palette_bits;
        tiles = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_MAIN, 3);
        for (i = 0; i < 768; ++i) {
            *tiles = (*tiles & 0x7ff) + palette;
            ++tiles;
        }
        func_ov007_02081664(1);
        func_ov005_0206650c(task);
        break;
    }
    }
}

void SceneMenu_StartBlend(int shrinking, int duration)
{
    SceneMenuBlendTask *task =
        (SceneMenuBlendTask *)func_ov005_0206659c((void *)SceneMenu_UpdateBlend, 11, 1);
    task->use_alternate_characters = 1 - shrinking;
    task->blend = 0;
    task->blend_step = _s32_div_f(0x10000, duration);
    if (shrinking)
        task->palette_bits = 0;
    else
        task->palette_bits = 4096;
    func_02036988(0x4000050, 1, 8, 0, 16);
    if (!shrinking)
        func_ov007_02081560(1, 12, (u16)task->palette_bits);
    else
        func_ov007_02081560(1, 8, (u16)task->palette_bits);
}
}
