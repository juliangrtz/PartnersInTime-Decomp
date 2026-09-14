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
/* Camera coordinates are Q12. For entity tracking, end_x/end_y are offsets.
 * The meaning of option_x/option_y remains under research. */
typedef struct FieldCameraMotion {
    union {
        u16 raw;
        struct { u16 active_axes : 2, unknown : 14; } bits;
        struct {
            u16 x_active : 1, y_active : 1, paused : 1, timed : 1;
            u16 option_x : 1, option_y : 1, braking : 1, unknown : 9;
        } options;
    } flags;
    u16 direction;
    int elapsed, speed, acceleration, deceleration, maximum_speed;
    fx32 start_x, start_y, end_x, end_y, vx, vy;
    struct FieldRuntimeEntity *entity;
} FieldCameraMotion;
typedef char FieldCameraMotionSize[sizeof(FieldCameraMotion) == 52 ? 1 : -1];

typedef char FieldLayerMotion_SizeCheck[sizeof(FieldLayerMotion) == 56 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void FieldArea_StopLayerMotion(FieldAreaContext *area, int layer);
u16 FieldArea_GetMotionDirection(FieldAreaContext *unused, fx32 dx, fx32 dy);
int FieldArea_TrackEntityForDuration(FieldAreaContext *area, struct FieldRuntimeEntity *entity,
    fx32 x, fx32 y, int duration, u8 option_x, u8 option_y, FieldCameraMotion *motion);
int FieldArea_TrackEntityAtSpeed(FieldAreaContext *area, struct FieldRuntimeEntity *entity,
    fx32 x, fx32 y, fx32 speed, u8 option_x, u8 option_y, FieldCameraMotion *motion);
int FieldArea_MoveCameraForDuration(FieldAreaContext *area, int relative, fx32 x, fx32 y,
    int duration, u8 option_x, u8 option_y, FieldCameraMotion *motion);
int FieldArea_MoveCameraWithProfile(FieldAreaContext *area, int relative, fx32 x, fx32 y,
    fx32 speed, fx32 acceleration, fx32 maximum, fx32 deceleration,
    u8 option_x, u8 option_y, FieldCameraMotion *motion);
void FieldArea_StopCameraMotion(FieldAreaContext *area, FieldCameraMotion *motion, int snap);
#ifdef __cplusplus
}
#endif
#endif
