#ifndef PIT_SHOP_PANEL_SLIDE_INTERNAL_H
#define PIT_SHOP_PANEL_SLIDE_INTERNAL_H
#include "shop_scene_internal.h"
#include <game/shop_panel_slide.h>

struct ShopPanelSlideTask {
    u8 unknown_00[36];
    int frames;
    s32 current, target, velocity, acceleration;
    u8 unknown_38[16];
};
typedef char ShopPanelSlideTaskSizeCheck[sizeof(ShopPanelSlideTask) == 72 ? 1 : -1];
extern "C" void func_ov005_0206650c(void *);
#endif
