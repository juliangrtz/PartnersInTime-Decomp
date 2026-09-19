#include "shop_panel_zoom_internal.h"

extern "C" void ShopPanelZoom_BeginClose(void)
{
    data_ov009_0207ea3c.panel_zooming = 1;
    ShopPanelZoomTask *task = (ShopPanelZoomTask *)func_ov005_0206659c(
        (void (*)(ShopMenuElement *))ShopPanelZoom_UpdateClose, 5, 1);
    task->step = data_ov009_0207ea3c.panel_scale_y / 6;
    task->delay = 1;
}
