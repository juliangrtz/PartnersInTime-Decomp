#include "save_write_effects_internal.h"
#include <game/save_menu_write.h>
#include <game/save_state_transfer.h>

extern SaveMenuBufferHeader data_0205e32c;
extern void func_ov008_02071bdc(int, int);
#define FULL_WORK (*(SaveMenuPanelValuesWorkPrefix *)data_ov008_02078290)
#define WORK FULL_WORK.menu
#define LIVE_SAVE ((SaveLiveTransferView *)gSaveData)

void GameOverScene_LoadResources(SaveSceneTask *task)
{
    int i;
    SaveMenu_LoadResources();
    {
        volatile u32 clear = 0;
        func_0203b7a0(clear, data_ov008_0207828c->pixels, 24576);
    }
    if (data_0205e32c.settings.occupied_slots & (1 << LIVE_SAVE->slot_select.bits.slot)) {
        SaveMenu_BuildStoredSummary(LIVE_SAVE->slot_select.bits.slot, WORK.summaries);
        WORK.summaries[0].available = 1;
        func_ov008_02071bdc(0, WORK.summaries[0].location);
        SaveMenu_LoadLocationName(0, WORK.summaries[0].location);
    } else {
        func_ov008_02071bdc(0, 0);
        SaveMenu_LoadLocationName(0, 0);
    }
    WORK.selected_panel = 0;
    for (i = 0; i < 14; ++i)
        FULL_WORK.panel_values[WORK.selected_panel][i] = 31;
    {
        int panel = 0;
        SaveLiveTransferView *live = LIVE_SAVE;
        for (; panel < 2; ++panel) {
            WRITE_WORK.panels[panel].slot = live->slot_select.bits.slot;
            WRITE_WORK.panels[panel].target_x = 589824;
            WRITE_WORK.panels[panel].target_y = 98304;
            WRITE_WORK.panels[panel].x = WRITE_WORK.panels[panel].target_x;
            WRITE_WORK.panels[panel].y = WRITE_WORK.panels[panel].target_y;
            WRITE_WORK.panels[panel].opacity = 126976;
            WRITE_WORK.panels[panel].scale = 4096;
        }
    }
    WRITE_WORK.panels[0].unknown_1c = 4096;
    WRITE_WORK.panels[1].unknown_1c = 0;
    WRITE_WORK.panels[0].unknown_02 = 1;
    WRITE_WORK.panels[1].unknown_02 = 0;
    Overlay5DisplayBg_SetAffine(1, 2, 4096, 4096, 0, 0, 0, 0, 0);
    Overlay5DisplayBg_SetAffine(1, 3, 4096, 4096, 0, 0, 0, 0, 0);
}
