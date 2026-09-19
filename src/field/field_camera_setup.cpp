/*
 * Camera movement (overlay 0, 0x02073110-0x020736A4).
 *
 * Moving the camera by a motion profile or over a fixed duration, and tracking
 * an entity at a speed or for a duration. Tracking re-reads the entity every
 * frame, so it follows something that is itself moving.
 */

#include <game/field_area_motion.h>
#include <game/field_entity.h>
extern "C" int _s32_div_f(int, int);
extern "C" void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);
extern "C" int FieldArea_MoveCameraWithProfile(FieldAreaContext *area, int relative, fx32 x, fx32 y,
                                               fx32 speed, fx32 acceleration, fx32 maximum,
                                               fx32 deceleration, u8 option_x, u8 option_y,
                                               FieldCameraMotion *motion) {
    int dx, dy, start_x, start_y;
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    if (relative == 1) {
        x += area->camera_x;
        y += area->camera_y;
    }
    if (x < 0)
        x = 0;
    if (x > area->unknown_243c[2])
        x = area->unknown_243c[2];
    if (y < 0)
        y = 0;
    if (y > area->unknown_243c[3])
        y = area->unknown_243c[3];
    motion->flags.options.x_active = x != area->camera_x;
    motion->flags.options.y_active = y != area->camera_y;
    if (motion->flags.bits.active_axes) {
        start_x = area->camera_x;
        start_y = area->camera_y;
        motion->flags.options.paused = 0;
        motion->flags.options.timed = 0;
        dx = x - start_x;
        motion->flags.options.option_x = option_x;
        motion->flags.options.option_y = option_y;
        dy = y - start_y;
        motion->flags.options.braking = 0;
        motion->entity = 0;
        motion->elapsed = 0;
        motion->start_x = area->camera_x;
        motion->start_y = area->camera_y;
        motion->end_x = x;
        motion->end_y = y;
        motion->direction = FieldArea_GetMotionDirection(area, dx, dy);
        motion->speed = speed;
        motion->acceleration = acceleration;
        motion->maximum_speed = maximum;
        motion->deceleration = deceleration;
        func_ov000_020be654(dx, dy, speed, &motion->vx, &motion->vy);
        if (motion->flags.options.x_active && !motion->vx)
            motion->vx = dx > 0 ? 1 : -1;
        if (motion->flags.options.y_active && !motion->vy)
            motion->vy = dy > 0 ? 1 : -1;
        return 1;
    }
    return 0;
}
extern "C" int FieldArea_MoveCameraForDuration(FieldAreaContext *area, int relative, fx32 x, fx32 y,
                                               int duration, u8 option_x, u8 option_y,
                                               FieldCameraMotion *motion) {
    int dx, dy, start_x, start_y;
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    if (relative == 1) {
        x += area->camera_x;
        y += area->camera_y;
    }
    if (x < 0)
        x = 0;
    if (x > area->unknown_243c[2])
        x = area->unknown_243c[2];
    if (y < 0)
        y = 0;
    if (y > area->unknown_243c[3])
        y = area->unknown_243c[3];
    motion->flags.options.x_active = x != area->camera_x;
    motion->flags.options.y_active = y != area->camera_y;
    start_x = area->camera_x;
    start_y = area->camera_y;
    motion->flags.options.paused = 0;
    motion->flags.options.timed = 1;
    dx = x - start_x;
    motion->flags.options.option_x = option_x;
    dy = y - start_y;
    motion->flags.options.option_y = option_y;
    motion->flags.options.braking = 0;
    motion->entity = 0;
    motion->elapsed = 0;
    motion->start_x = area->camera_x;
    motion->start_y = area->camera_y;
    motion->end_x = x;
    motion->end_y = y;
    motion->direction = FieldArea_GetMotionDirection(area, dx, dy);
    motion->speed = duration;
    if (motion->flags.options.x_active)
        motion->vx = _s32_div_f(dx, duration);
    if (motion->flags.options.y_active)
        motion->vy = _s32_div_f(dy, duration);
    return 1;
}
extern "C" int FieldArea_TrackEntityAtSpeed(FieldAreaContext *area, FieldRuntimeEntity *entity,
                                            fx32 x, fx32 y, int speed, u8 option_x, u8 option_y,
                                            FieldCameraMotion *motion) {
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    motion->flags.options.x_active = area->camera_x - 0x80000 != entity->position_x + x;
    motion->flags.options.y_active =
        area->camera_y - 0x88000 != y + (entity->position_y - entity->position_z);
    if (motion->flags.options.x_active || motion->flags.options.y_active) {
        motion->flags.options.paused = 0;
        motion->flags.options.timed = 0;
        motion->flags.options.option_x = option_x;
        motion->flags.options.option_y = option_y;
        motion->flags.options.braking = 0;
        motion->entity = entity;
        motion->elapsed = 0;
        motion->speed = speed;
        motion->acceleration = 0;
        motion->maximum_speed = speed;
        motion->deceleration = 0;
        motion->end_x = x;
        motion->end_y = y;
        return 1;
    }
    return 0;
}
extern "C" int FieldArea_TrackEntityForDuration(FieldAreaContext *area, FieldRuntimeEntity *entity,
                                                fx32 x, fx32 y, int duration, u8 option_x,
                                                u8 option_y, FieldCameraMotion *motion) {
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    motion->flags.bits.active_axes = 1;
    motion->flags.options.paused = 0;
    motion->flags.options.timed = 1;
    motion->flags.options.option_x = option_x;
    motion->flags.options.option_y = option_y;
    motion->flags.options.braking = 0;
    motion->entity = entity;
    motion->elapsed = 0;
    motion->speed = duration;
    motion->end_x = x;
    motion->end_y = y;
    return 1;
}
