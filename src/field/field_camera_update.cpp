/*
 * Camera motion updates (overlay 0, 0x02072A9C-0x02073110).
 * Resolve tracked targets and stop/clamp completed axes, then advance the
 * camera origin using timed or accelerated motion. Tracking constructs a
 * temporary absolute motion each frame to account for the moving entity.
 */
extern "C" {
#include <nitro/fx.h>
}
#include <game/field_area_motion.h>
#include <game/field_entity.h>
extern "C" void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);
extern "C" void FieldArea_OffsetCameraOrigin(FieldAreaContext *, fx32, fx32);
extern "C" void FieldArea_AdvanceCameraMotion(FieldAreaContext *area, FieldCameraMotion *motion)
{
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    if (!motion->flags.bits.active_axes || motion->flags.options.paused)
        return;
    if (motion->entity) {
        FieldCameraMotion temporary;
        fx32 x = motion->entity->position_x + motion->end_x - 0x80000;
        fx32 y =
            motion->end_y + (motion->entity->position_y - motion->entity->position_z) - 0x88000;
        if (!motion->flags.options.timed) {
            FieldArea_MoveCameraWithProfile(area, 0, x, y, motion->speed, motion->acceleration,
                                            motion->maximum_speed, motion->deceleration,
                                            motion->flags.options.option_x != 0,
                                            motion->flags.options.option_y != 0, &temporary);
        } else {
            int duration = motion->speed - motion->elapsed;
            if (duration <= 0)
                duration = 1;
            FieldArea_MoveCameraForDuration(area, 0, x, y, duration,
                                            motion->flags.options.option_x != 0,
                                            motion->flags.options.option_y != 0, &temporary);
        }
        FieldArea_AdvanceCameraMotion(area, &temporary);
    } else {
        if (!motion->flags.options.timed && motion->flags.options.braking) {
            motion->speed += motion->deceleration;
            if (motion->speed < -motion->deceleration)
                motion->speed = -motion->deceleration;
            func_ov000_020be654(motion->end_x - area->camera_x, motion->end_y - area->camera_y,
                                motion->speed, &motion->vx, &motion->vy);
        }
        fx32 dx = motion->flags.options.x_active ? motion->vx : 0;
        fx32 dy = motion->flags.options.y_active ? motion->vy : 0;
        FieldArea_OffsetCameraOrigin(area, dx, dy);
        if (!motion->flags.options.timed && !motion->flags.options.braking) {
            if (motion->acceleration > 0 && motion->speed < motion->maximum_speed) {
                motion->speed += motion->acceleration;
                if (motion->speed > motion->maximum_speed)
                    motion->speed = motion->maximum_speed;
                func_ov000_020be654(motion->end_x - area->camera_x, motion->end_y - area->camera_y,
                                    motion->speed, &motion->vx, &motion->vy);
            }
        }
    }
    ++motion->elapsed;
}

extern "C" int FieldArea_UpdateCameraTarget(FieldAreaContext *area, FieldCameraMotion *motion)
{
    if (!motion)
        motion = (FieldCameraMotion *)area->unknown_2b90;
    if (motion->flags.bits.active_axes && !motion->flags.options.paused) {
        if (motion->flags.options.option_x) {
            if ((!area->camera_x || area->camera_x == area->unknown_243c[2]) &&
                (!area->camera_y || area->camera_y == area->unknown_243c[3])) {
                motion->flags.bits.active_axes = 0;
                return 0;
            }
        }
        if (motion->flags.options.option_y) {
            if (motion->entity) {
                if (!motion->flags.options.timed) {
                    /* unknown_243c[0..1] hold the previous camera position. */
                    fx32 x = motion->entity->position_x + motion->end_x - 0x80000;
                    if ((area->unknown_243c[0] < x && area->camera_x < x) ||
                        (area->unknown_243c[0] > x && area->camera_x > x))
                        return 1;
                    area->camera_x = x;
                    fx32 y = motion->end_y +
                             (motion->entity->position_y - motion->entity->position_z) - 0x88000;
                    if ((area->unknown_243c[1] < y && area->camera_y < y) ||
                        (area->unknown_243c[1] > y && area->camera_y > y))
                        return 1;
                    area->camera_y = y;
                    motion->flags.bits.active_axes = 0;
                } else if (motion->elapsed == motion->speed) {
                    area->camera_x = motion->entity->position_x + motion->end_x - 0x80000;
                    area->camera_y = motion->end_y +
                                     (motion->entity->position_y - motion->entity->position_z) -
                                     0x88000;
                    motion->flags.bits.active_axes = 0;
                }
            } else if (!motion->flags.options.timed) {
                if (motion->flags.options.x_active) {
                    if ((motion->start_x > motion->end_x && area->camera_x <= motion->end_x) ||
                        (motion->start_x < motion->end_x && area->camera_x >= motion->end_x)) {
                        area->camera_x = motion->end_x;
                        motion->flags.options.x_active = 0;
                    }
                }
                if (motion->flags.options.y_active) {
                    if ((motion->start_y > motion->end_y && area->camera_y <= motion->end_y) ||
                        (motion->start_y < motion->end_y && area->camera_y >= motion->end_y)) {
                        area->camera_y = motion->end_y;
                        motion->flags.options.y_active = 0;
                    }
                }
                if (motion->flags.bits.active_axes && motion->deceleration < 0) {
                    /* Sum the discrete speed steps, including the final nonpositive one. */
                    int speed = motion->speed;
                    int distance = 0;
                    do {
                        speed += motion->deceleration;
                        distance += speed;
                    } while (speed > 0);
                    fx32 dx = motion->end_x - area->camera_x;
                    fx32 dy = motion->end_y - area->camera_y;
                    fx32 remaining = FX_Sqrt((fx32)(((s64)dx * dx + 2048) >> 12) +
                                             (fx32)(((s64)dy * dy + 2048) >> 12));
                    if (distance >= remaining)
                        motion->flags.options.braking = 1;
                    else
                        motion->flags.options.braking = 0;
                }
            } else if (motion->elapsed == motion->speed) {
                area->camera_x = motion->end_x;
                area->camera_y = motion->end_y;
                motion->flags.bits.active_axes = 0;
            }
        }
        if (!motion->flags.bits.active_axes) {
            if (area->camera_x < 0)
                area->camera_x = 0;
            if (area->camera_x > area->unknown_243c[2])
                area->camera_x = area->unknown_243c[2];
            if (area->camera_y < 0)
                area->camera_y = 0;
            if (area->camera_y > area->unknown_243c[3])
                area->camera_y = area->unknown_243c[3];
        }
    }
    return motion->flags.bits.active_axes != 0;
}
