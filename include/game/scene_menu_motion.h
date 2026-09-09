#ifndef PIT_GAME_SCENE_MENU_MOTION_H
#define PIT_GAME_SCENE_MENU_MOTION_H
#include <nitro.h>

/* Positions and velocities use four fractional bits; scale uses eight. */
typedef struct SceneMenuMotionParent {
    u8 unknown_00[40];
    int pending;
} SceneMenuMotionParent;
typedef struct SceneMenuTweenTask {
    u8 unknown_00[16];
    SceneMenuMotionParent *parent;
    u8 unknown_14[12];
    int phase;
    int duration;
    u16 elapsed, scale, target_scale;
    s16 scale_velocity, scale_acceleration;
    s16 x, y, velocity_x, velocity_y, acceleration_x, acceleration_y, target_x, target_y;
    u8 unknown_42[6];
} SceneMenuTweenTask;
typedef struct SceneMenuBlendTask {
    u8 unknown_00[32];
    int phase;
    int unknown_24;
    int use_alternate_characters;
    int blend;
    int blend_step;
    int palette_bits;
    u8 unknown_38[16];
} SceneMenuBlendTask;

typedef char SceneMenuMotionParent_SizeCheck[sizeof(SceneMenuMotionParent) == 44 ? 1 : -1];
typedef char SceneMenuTweenTask_SizeCheck[sizeof(SceneMenuTweenTask) == 72 ? 1 : -1];
typedef char SceneMenuBlendTask_SizeCheck[sizeof(SceneMenuBlendTask) == 72 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void SceneMenu_UpdateZoom(SceneMenuTweenTask *task);
void SceneMenu_UpdateBlend(SceneMenuBlendTask *task);
void SceneMenu_StartBlend(int shrinking, int duration);
void SceneMenu_ApplyScroll(SceneMenuTweenTask *task);
void SceneMenu_UpdateScroll(SceneMenuTweenTask *task);
void SceneMenu_StartScroll(int x, int y, int duration);
#ifdef __cplusplus
}
#endif
#endif
