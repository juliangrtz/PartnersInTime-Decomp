#ifndef PIT_SCENE_MENU_MOTION_INTERNAL_H
#define PIT_SCENE_MENU_MOTION_INTERNAL_H
#include <game/scene_menu_motion.h>
extern "C" {
#include <game/overlay005_display.h>
/* This view starts at the native scroll-state anchor inside the menu work. */
typedef struct SceneMenuScrollState {
    u8 unknown_00[50];
    s16 x, y;
    u8 unknown_36[2];
} SceneMenuScrollState;
typedef char SceneMenuScrollState_SizeCheck[sizeof(SceneMenuScrollState) == 56 ? 1 : -1];
extern SceneMenuScrollState data_ov007_020906f0;
extern void *func_ov005_0206659c(void *, int, int);
extern void func_ov005_0206650c(void *);
extern int _s32_div_f(int, int);
extern void func_02036988(int, int, int, int, int);
extern void func_ov007_02081560(int, int, u16);
extern void func_ov007_02081664(int);
}
#endif
