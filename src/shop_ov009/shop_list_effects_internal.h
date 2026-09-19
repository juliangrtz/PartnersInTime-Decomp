#ifndef PIT_SHOP_LIST_EFFECTS_INTERNAL_H
#define PIT_SHOP_LIST_EFFECTS_INTERNAL_H
#include "shop_navigation_internal.h"

struct ShopListScaleTask {
    u8 unknown_00[36];
    int frames;
    s32 current, target, step;
    int delay;
    u8 unknown_38[16];
};
struct ShopListPointTask {
    u8 unknown_00[40];
    int point, priority;
    u8 unknown_30[24];
};
typedef char ShopListScaleTaskSizeCheck[sizeof(ShopListScaleTask) == 72 ? 1 : -1];
typedef char ShopListPointTaskSizeCheck[sizeof(ShopListPointTask) == 72 ? 1 : -1];
extern "C" {
void func_ov005_0206650c(void *);
void func_ov005_02069bcc(int sound, u16 delay, u16 wait, u16 priority);
}
#endif
