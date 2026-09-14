#ifndef PIT_GAME_FIELD_AREA_MOTION_H
#define PIT_GAME_FIELD_AREA_MOTION_H
#include <game/field_area.h>
typedef struct FieldLayerMotion {
    union {
        u16 raw;
        struct {
            u16 active_axes : 2, unknown : 14;
        } axes;
        struct {
            u16 x_active : 1, y_active : 1, paused : 1, timed : 1, braking : 1, stop : 1,
                unknown : 10;
        } bits;
    } flags;
    u16 direction;
    int elapsed, speed, acceleration, deceleration, maximum_speed;
    fx32 target_x, target_y, vx, vy, x, y, previous_x, previous_y;
} FieldLayerMotion;
typedef struct FieldCameraMotion {
    union {
        u16 raw;
        struct {
            u16 active_axes : 2, unknown : 14;
        } bits;
    } flags;
    u8 unknown02[30];
    fx32 end_x, end_y;
    u8 unknown28[12];
} FieldCameraMotion;
typedef char FieldCameraMotionSize[sizeof(FieldCameraMotion) == 52 ? 1 : -1];

typedef char FieldLayerMotion_SizeCheck[sizeof(FieldLayerMotion) == 56 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void FieldArea_StopLayerMotion(FieldAreaContext *area, int layer);
u16 FieldArea_GetMotionDirection(FieldAreaContext *unused, fx32 dx, fx32 dy);
void FieldArea_StopCameraMotion(FieldAreaContext *area, FieldCameraMotion *motion, int snap);
#ifdef __cplusplus
}
#endif
#endif
