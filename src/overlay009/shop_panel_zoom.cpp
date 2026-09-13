#include "shop_panel_zoom_internal.h"
#define WORK data_ov009_0207ea3c

extern "C" void ShopPanelZoom_UpdateOpen(ShopPanelZoomTask *task)
{
    if (task->delay) {
        --task->delay;
    } else {
        WORK.panel_scale_y += task->step;
        if (WORK.panel_scale_y > 4096) WORK.panel_scale_y = 4096;
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_MAIN, 2, 4096, WORK.panel_scale_y,
                                   0, 128, 96, 0, 0);
        Overlay5Display_EnablePlanes(DISPLAY_ENGINE_MAIN, 4);
        ShopItemPanel_SetVector334((ShopItemPanel *)data_ov009_0207ea20,
            (393216 - 32 * WORK.panel_scale_y) / 4096,
            (32 * WORK.panel_scale_y + 393216) / 4096);
        if (WORK.panel_scale_y == 4096) {
            WORK.panel_zooming = 0;
            func_ov005_0206650c(task);
        }
    }
}

extern "C" void ShopPanelZoom_UpdateClose(ShopPanelZoomTask *task)
{
    if (task->delay) {
        --task->delay;
    } else {
        WORK.panel_scale_y -= task->step;
        if (WORK.panel_scale_y < 0) WORK.panel_scale_y = 0;
        Overlay5DisplayBg_SetAffine(DISPLAY_ENGINE_MAIN, 2, 4096, WORK.panel_scale_y,
                                   0, 128, 96, 0, 0);
        ShopItemPanel_SetVector334((ShopItemPanel *)data_ov009_0207ea20,
            (393216 - 32 * WORK.panel_scale_y) / 4096,
            (32 * WORK.panel_scale_y + 393216) / 4096);
        if (!WORK.panel_scale_y) {
            Overlay5Display_DisablePlanes(DISPLAY_ENGINE_MAIN, 4);
            WORK.panel_zooming = 0;
            WORK.panel_visible = 0;
            func_ov005_0206650c(task);
        }
    }
}
