#include "pause_scene_internal.h"
#include <game/pause_transition.h>
#include <game/pause_navigation.h>
#include <game/overlay005_resource.h>
extern "C" {
#include <game/overlay007_party.h>
#include <game/pause_menu_label.h>
}

struct PauseTransitionProgressTask {
    u8 unknown_00[32];
    s32 phase;
    s32 unknown_24, angle;
    s32 step;
    u8 unknown_30[24];
};
typedef char TransitionProgressTaskSize[sizeof(PauseTransitionProgressTask) == 72 ? 1 : -1];

extern "C" {
void func_ov005_0206650c(void *);
void func_ov007_0206e0d4(int, int, int);
}

struct PauseShutterPanelTask {
    u8 unknown_00[32];
    s32 phase, delay, panel, tile;
    u8 unknown_30[24];
};
typedef char ShutterPanelTaskSize[sizeof(PauseShutterPanelTask) == 72 ? 1 : -1];

extern "C" {
Overlay5ObjectSprite *Overlay5ResourceB_Get(void *);
void func_ov005_0206786c(int, int, int, int, int, int);
void func_ov005_020692c0(Overlay5ObjectSprite *, int, int, int);
void func_ov005_02069084(void *, int);
void func_ov005_020679fc(int, int, int, int);
void func_ov005_020679a4(int, int, int);
void func_02036964(vu16 *, int, int);
}

struct PauseTransitionControllerTask {
    u8 unknown_00[16];
    PauseTransitionControllerTask *owner;
    u8 unknown_14[12];
    s32 phase, delay, pending, step;
    u8 unknown_30[24];
};
typedef char TransitionControllerTaskSize[sizeof(PauseTransitionControllerTask) == 72 ? 1 : -1];

extern "C" {
extern u8 data_ov007_020901e8[];
void func_ov007_02077154(int);
}

struct PauseFadeInTask {
    u8 unknown_00[32];
    s32 phase, brightness, unknown_28;
    s32 unknown_2c, unknown_30, unknown_34;
    u8 unknown_38[16];
};
typedef char FadeInTaskSize[sizeof(PauseFadeInTask) == 72 ? 1 : -1];

extern "C" {
void func_ov007_02076e30(int);
void func_ov007_0207fe90(int);
void func_ov007_02080c40(void);
void func_ov007_0207b2dc(int, int);
void func_ov005_02066358(PauseMenuElement *, void (*)(PauseMenuElement *), int);
int SceneScript_StartPrimary(u8 *);
}

struct PauseExitTask {
    u8 unknown_00[32];
    s32 phase, brightness;
    u8 unknown_28[32];
};
typedef char PauseExitTaskSize[sizeof(PauseExitTask) == 72 ? 1 : -1];


/* First word of the shared transition workspace; preserve its raw byte view. */
#define PROGRESS (*(s32 *)data_ov007_0208e1e8)

extern "C" void PauseScene_FadeInTask(PauseFadeInTask *task)
{
    switch (task->phase) {
    case 0:
        func_ov005_020671e4(ARCHIVE);
        ++task->phase;
        break;
    case 1:
        func_ov007_02076e30(0);
        func_ov007_02077154(3);
        PROGRESS = 0;
        data_ov007_020901e8[0x404] = 3;
        func_ov007_0207fe90(0);
        Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, 12);
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_MAIN, 4);
        PauseList_InitGraphics((Overlay7Party *)data_ov007_0208e1e4);
        func_ov007_02080c40();
        PauseMenuLabel_DrawPage((u8)WORK.selected_menu);
        func_ov007_0207b2dc(1, 0);
        task->brightness = 0;
        ++task->phase;
        break;
    case 2:
        ++task->brightness;
        func_ov005_0206781c(2, task->brightness - 16);
        if (task->brightness >= 16) {
            if (WORK.tutorial) SceneScript_StartPrimary(data_ov007_020a6b90);
            data_ov007_0208e1e0->phase = 2;
            func_ov005_02066358((PauseMenuElement *)task, (void (*)(PauseMenuElement *))PauseMenu_UpdateTask, 0);
            task->unknown_2c = 0;
            task->unknown_30 = 0;
            task->unknown_34 = 0;
            task->phase = 100;
        }
        break;
    }
}

extern "C" void PauseTransition_EntryProgressTask(PauseTransitionControllerTask *task)
{
    switch (task->phase) {
    case 0:
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, 12);
        PROGRESS = 0;
        func_ov007_0206e0d4(0, 0, 0);
        task->delay = 4;
        ++task->phase;
        break;
    case 1: {
        if (task->delay) {
            --task->delay;
            if (!task->delay) {
                PauseTransitionProgressTask *window =
                    (PauseTransitionProgressTask *)func_ov005_0206659c(
                        (void (*)(PauseMenuElement *))PauseTransition_WindowTask,
                        11, 1);
                window->step = task->step;
            }
            return;
        }
        int next = PROGRESS + task->step;
        int wrapped = next & 0xffff;
        PROGRESS = next;
        PROGRESS = wrapped;
        if (wrapped > 45056) {
            func_ov007_0206e0d4(0, wrapped, 0);
            return;
        }
        task->phase = 10;
        /* The native phase-1 completion falls through to phase 10. */
    }
    case 10:
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, 12);
        if (task->delay) {
            --task->delay;
        } else {
            if (task->owner->pending) --task->owner->pending;
            PROGRESS = 16384;
            func_ov007_0206e0d4(1, 16384, 192);
            func_02036964((vu16 *)0x04001050, 12, -16);
            Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, 12);
            Overlay5Display_DisablePlanes(DISPLAY_ENGINE_MAIN, 4);
            func_ov007_02077154(1);
            ++task->phase;
        }
        break;
    case 11: {
        int next = PROGRESS + task->step;
        PROGRESS = next;
        if (next <= 0) {
            func_ov007_02077154(2);
            PROGRESS = 0;
            ++task->phase;
        }
        func_02036964((vu16 *)0x04001050, 12,
                     -16 * PROGRESS / 16384);
        func_ov007_0206e0d4(1, PROGRESS, 192);
        break;
    }
    case 12:
        func_ov007_02077154(3);
        data_ov007_020901e8[0x404] = 3;
        if (task->owner->pending) --task->owner->pending;
        func_ov005_0206650c(task);
        break;
    }
}

extern "C" void PauseTransition_EntryPanelTask(PauseShutterPanelTask *task)
{
    Overlay5ObjectSprite *sprite = Overlay5ResourceB_Get(task);
    int width, height;
    switch (task->phase) {
    case 0:
        if (PROGRESS == 16384) {
            sprite->screen = DISPLAY_ENGINE_SUB;
            task->tile += (u32)WORK.sub_allocation.offset >> 6;
            ++task->phase;
            return;
        }
        PauseTransition_Project((task->panel << 6) - 128, 172,
                           PROGRESS,
                           &sprite->x, &sprite->y, &width, &height);
        sprite->x += 128 * 4096;
        height = (3 * height + 128) / 2;
        if (!task->panel) sprite->x += 4096;
        if (task->panel == 3) sprite->x -= 4096;
        if (height > 224) {
            sprite->attributes_1_bits.tile = task->tile;
        } else {
            sprite->attributes_1_bits.tile = task->tile + 2048;
            height = 192;
        }
        sprite->target_x = (width << 9) - 262144;
        sprite->target_y = (height << 8) - 131072;
        func_ov005_020692c0(sprite, (u16)width, (u16)height, 0);
        func_ov005_02069084(sprite, 0);
        break;
    case 1:
        if (PROGRESS <= 0) {
            func_ov005_0206650c(task);
            return;
        }
        PauseTransition_Project((task->panel << 6) - 128, -190,
                           PROGRESS,
                           &sprite->x, &sprite->y, &width, &height);
        sprite->x += 128 * 4096;
        sprite->y += 192 * 4096;
        height *= 2;
        if (!task->panel) sprite->x += 4096;
        if (task->panel == 3) sprite->x -= 4096;
        if (height > 240)
            sprite->attributes_1_bits.tile = task->tile;
        else if (height > 208)
            sprite->attributes_1_bits.tile = task->tile + 2048;
        else
            sprite->attributes_1_bits.tile = task->tile + 4096;
        sprite->target_x = (width << 9) - 262144;
        sprite->target_y = (height << 8) - 131072;
        func_ov005_020692c0(sprite, (u16)width, (u16)height, 0);
        func_ov005_02069084(sprite, 0);
        break;
    }
}

extern "C" void PauseTransition_WindowTask(PauseTransitionProgressTask *task)
{
    switch (task->phase) {
    case 0:
        func_ov005_020679fc(0, 0, 31, 1);
        func_ov005_020679a4(0, 31, 0);
        func_ov005_0206786c(0, 0, 0, 0, 255, 184);
        Overlay5Display_SetVisibleWindows(DISPLAY_ENGINE_MAIN, DISPLAY_WINDOW_0);
        func_02036964((vu16 *)0x04000050, 8, -8);
        task->angle = 0;
        ++task->phase;
        break;
    case 1: {
        int x, y, width, height;
        task->angle += task->step;
        task->angle &= 0xffff;
        PauseTransition_Project(0, 192, task->angle, &x, &y, &width, &height);
        y += 524288;
        y >>= 12;
        if (y <= 0) {
            func_02036964((vu16 *)0x04000050, 8, 0);
            Overlay5Display_DisableWindows(DISPLAY_ENGINE_MAIN, DISPLAY_WINDOW_0);
            func_ov005_0206650c(task);
            return;
        }
        if (y > 184) y = 184;
        func_ov005_0206786c(0, 0, 0, 0, 255, y);
        break;
    }
    }
}

extern "C" void PauseTransition_ExitProgressTask(PauseTransitionProgressTask *task)
{
    switch (task->phase) {
    case 0:
        PROGRESS = 0;
        func_ov007_0206e0d4(1, 0, 192);
        ++task->phase;
        break;
    case 1: {
        int next = PROGRESS + task->step;
        int wrapped = next & 0xffff;
        PROGRESS = next;
        PROGRESS = wrapped;
        if (wrapped >= 16384) {
            func_ov005_0206650c(task);
            return;
        }
        func_ov007_0206e0d4(1, wrapped, 192);
        break;
    }
    }
}

extern "C" void PauseTransition_ExitPanelTask(PauseShutterPanelTask *task)
{
    Overlay5ObjectSprite *sprite = Overlay5ResourceB_Get(task);
    if (task->phase) return;
    if (task->delay) {
        --task->delay;
        return;
    }
    int angle = PROGRESS;
    if (angle >= 16384) {
        func_ov005_0206650c(task);
        return;
    }
    int width, height;
    PauseTransition_Project((task->panel << 6) - 128, -190, angle,
                       &sprite->x, &sprite->y, &width, &height);
    sprite->x += 128 * 4096;
    sprite->y += 192 * 4096;
    height *= 2;
    if (!task->panel)
        func_ov005_0206786c(1, 0, 0, sprite->y / 4096, 255, 192);
    if (!task->panel) sprite->x += 4096;
    if (task->panel == 3) sprite->x -= 4096;
    if (height > 240)
        sprite->attributes_1_bits.tile = task->tile;
    else if (height > 208)
        sprite->attributes_1_bits.tile = task->tile + 2048;
    else
        sprite->attributes_1_bits.tile = task->tile + 4096;
    sprite->target_x = (width << 9) - 262144;
    sprite->target_y = (height << 8) - 131072;
    func_ov005_020692c0(sprite, (u16)width, (u16)height, 0);
    func_ov005_02069084(sprite, 0);
}

extern "C" void PauseScene_FadeOutTask(PauseExitTask *task)
{
    switch (task->phase) {
    case 0:
        task->brightness = 0;
        ++task->phase;
        break;
    case 1:
        --task->brightness;
        func_ov005_0206781c(2, task->brightness);
        if (task->brightness == -16) {
            data_ov007_0208e1e0->phase = 6;
            func_ov005_0206650c(task);
        }
        break;
    }
}

extern "C" void PauseScene_PrepareExitTask(PauseExitTask *task)
{
    switch (task->phase) {
    case 0:
        /* Preserve the native signed -1 access and the existing raw byte view. */
        *(s8 *)&WORK.unknown110 = -1;
        func_ov005_020671e4(ARCHIVE);
        ++task->phase;
        break;
    case 1:
        PauseScene_RequestExit(data_ov007_0208e1e0, 1);
        break;
    }
}
