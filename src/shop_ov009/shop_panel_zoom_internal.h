#ifndef PIT_SHOP_PANEL_ZOOM_INTERNAL_H
#define PIT_SHOP_PANEL_ZOOM_INTERNAL_H
#include "shop_scene_internal.h"
#include <game/shop_panel_zoom.h>

struct ShopPanelZoomTask {
    u8 unknown_00[40];
    int step, delay;
    u8 unknown_30[24];
};
typedef char ShopPanelZoomTaskSizeCheck[sizeof(ShopPanelZoomTask) == 72 ? 1 : -1];
extern "C" {
void func_ov005_0206650c(void *);
}
#endif
