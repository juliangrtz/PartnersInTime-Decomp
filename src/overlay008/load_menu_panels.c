#include "save_write_effects_internal.h"
extern const u8 data_ov008_02077f30[2][2];
extern void func_ov008_0206d47c(MenuElement *);
extern void func_ov008_0206d1d0(MenuElement *);
#define ENTRY_WORK (*(SaveMenuEntryWorkPrefix *)data_ov008_02078290)

void LoadMenu_InitializePanels(void)
{
    int i;
    for (i = 0; i < 2; ++i) {
        SaveMenuTransferTask *task;
        WRITE_WORK.panels[i].slot = i;
        WRITE_WORK.panels[i].target_x = data_ov008_02077f30[i][0] << 12;
        WRITE_WORK.panels[i].target_y = data_ov008_02077f30[i][1] << 12;
        WRITE_WORK.panels[i].x = WRITE_WORK.panels[i].target_x;
        WRITE_WORK.panels[i].y = WRITE_WORK.panels[i].target_y;
        WRITE_WORK.panels[i].opacity = 126976;
        WRITE_WORK.panels[i].scale = 4096;
        if (ENTRY_WORK.selected_panel == i)
            WRITE_WORK.panels[i].unknown_1c = 0;
        else
            WRITE_WORK.panels[i].unknown_1c = 4096;
        task = func_ov005_0206659c(func_ov008_0206d47c, 2, 1);
        task->arguments[0] = i;
        task->arguments[1] = ENTRY_WORK.selected_panel;
        task = func_ov005_0206659c(func_ov008_0206d1d0, 2, 1);
        task->arguments[0] = i;
        task->arguments[1] = ENTRY_WORK.selected_panel;
        task->arguments[4] = 0;
        task->arguments[5] = 0;
    }
    if (!ENTRY_WORK.selected_panel) {
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 3, 2);
    } else {
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 2);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 3, 1);
    }
}
