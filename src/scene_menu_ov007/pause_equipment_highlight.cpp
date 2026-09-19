#include "pause_scene_internal.h"
#include <game/overlay007_party.h>
#include <game/pause_equipment_highlight.h>

struct PauseEquipmentHighlightTask {
    u8 unknown_00[32];
    s32 phase, unknown_24;
    s32 alpha, target, step;
    u8 unknown_34[20];
};
typedef char HighlightTaskSize[sizeof(PauseEquipmentHighlightTask) == 72 ? 1 : -1];

extern "C" {
extern HighlightItemView data_ov007_020907f0;
void func_02036988(vu32 *, int, int, int, int);
void func_ov005_0206650c(void *);
u8 func_ov007_02075400(Overlay7Party *);
int func_ov007_02074b44(Overlay7Party *, int, int, u32);
}
#define HIGHLIGHT_STATE WORK.equipment_highlight_state
#define PARTY ((Overlay7Party *)data_ov007_0208e1e4)

extern "C" void PauseEquipmentHighlight_Update(PauseEquipmentHighlightTask *task)
{
    if (HIGHLIGHT_STATE == -1) {
        HIGHLIGHT_STATE = 0;
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
        REG16(0x04001050) = 0;
        func_ov005_0206650c(task);
        return;
    }
    if (task->phase < 100 && !HIGHLIGHT_STATE)
        task->phase = 100;
    switch (task->phase) {
    case 0:
        task->step = (task->target - task->alpha) / 6;
        ++task->phase;
        /* Fall through to the first fade-in update. */
    case 1:
        task->alpha += task->step;
        if (task->alpha >= 0x8000) {
            task->alpha = 0x8000;
            ++task->phase;
        }
        func_02036988((vu32 *)0x04001050, 2, 31,
                     task->alpha / 4096, 16 - task->alpha / 4096);
        break;
    case 100:
        task->step = -task->alpha / 6;
        ++task->phase;
        /* Fall through to the first fade-out update. */
    case 101:
        task->alpha += task->step;
        if (task->alpha <= 0) {
            task->alpha = 0;
            Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
            REG16(0x04001050) = 0;
            func_ov005_0206650c(task);
            return;
        }
        func_02036988((vu32 *)0x04001050, 2, 31,
                     task->alpha / 4096, 16 - task->alpha / 4096);
        break;
    }
    int item = data_ov007_020907f0.item;
    if (!item) {
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
        return;
    }
    int kind = func_ov007_02075400(PARTY);
    if (func_ov007_02074b44(PARTY, kind, item, 0)) {
        if (WORK.available[2]) {
            Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 1, 0);
            Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
        } else {
            Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
        }
    } else if (WORK.available[0]) {
        Overlay5DisplayBg_SetVerticalOffset(DISPLAY_ENGINE_SUB, 1, -72);
        Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
    } else {
        Overlay5Display_DisablePlanes(DISPLAY_ENGINE_SUB, DISPLAY_PLANE_BG1);
    }
}

extern "C" void PauseEquipmentHighlight_Start(void)
{
    if (HIGHLIGHT_STATE)
        return;
    HIGHLIGHT_STATE = 1;
    func_02036988((vu32 *)0x04001050, 2, 31, 0, 16);
    PauseEquipmentHighlightTask *task = (PauseEquipmentHighlightTask *)func_ov005_0206659c(
        (void (*)(PauseMenuElement *))PauseEquipmentHighlight_Update, 11, 1);
    task->alpha = 0;
    task->target = 0x8000;
}

extern "C" void PauseEquipmentHighlight_Stop(void)
{
    HIGHLIGHT_STATE = 0;
}
