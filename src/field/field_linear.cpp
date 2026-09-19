/*
 * Linear entity motion (overlay 0, 0x020A5BF4-0x020A6690).
 *
 * Straight-line movement with acceleration: a controller carries the target,
 * the current speed and the acceleration and deceleration rates, and
 * UpdatePosition advances it one frame. Following mode re-reads the target from
 * another entity every frame so the mover tracks something that is itself
 * moving.
 *
 * CheckCompletion decides when to stop, comparing the remaining distance with
 * the distance the current speed still needs to brake in - which is why this
 * file needs FX_Sqrt and pulls nitro/fx.h in with C linkage before anything
 * else.
 */

/* nitro/fx.h has no linkage guard of its own, so it has to be seen inside an
 * extern "C" block before any header pulls it in at C++ scope. */
extern "C" {
#include <nitro/fx.h>
}
#include <game/field_transform.h>
extern "C" {
extern u16 FX_Atan2Idx(fx32, fx32);
}
#include <game/field_linear.h>
extern "C" {
extern void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);
#define LINEAR_DEFAULT(entity) (&(entity)->linear_controller)

int FieldLinear_Start(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, fx32 speed, fx32 acceleration,
                      fx32 maximum_speed, fx32 deceleration, u8 stop, FieldLinearController *movement)
{
    fx32 dx, dy;
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (relative == 1) {
        x += entity->position_x;
        y += entity->position_y;
    }
    movement->axes.x = x != entity->position_x;
    movement->axes.y = y != entity->position_y;
    if (movement->axes.x || movement->axes.y) {
        dx = x - entity->position_x;
        dy = y - entity->position_y;
        movement->bits.paused = 0;
        movement->bits.fixed_duration = 0;
        movement->bits.stop_at_destination = stop;
        movement->bits.braking = 0;
        movement->target = 0;
        movement->elapsed_frames = 0;
        movement->bits.stop_mask_a = 0;
        movement->bits.stop_mask_b = 0;
        movement->start_x = entity->position_x;
        movement->start_y = entity->position_y;
        movement->destination_x = x;
        movement->destination_y = y;
        movement->direction = FX_Atan2Idx(dy, dx) + 0x4000;
        if (speed) {
            movement->timing.speed = speed;
            movement->acceleration = acceleration;
            movement->maximum_speed = maximum_speed;
            movement->deceleration = deceleration;
        } else {
            movement->timing.speed = entity->locomotion.starting_speed;
            movement->acceleration = entity->locomotion.acceleration;
            movement->maximum_speed = entity->locomotion.maximum_speed;
            movement->deceleration = entity->locomotion.deceleration;
            speed = entity->locomotion.starting_speed;
        }
        func_ov000_020be654(dx, dy, speed, &movement->velocity_x, &movement->velocity_y);
        if (movement->axes.x && !movement->velocity_x)
            movement->velocity_x = dx;
        if (movement->axes.y && !movement->velocity_y)
            movement->velocity_y = dy;
        return 1;
    }
    return 0;
}
int FieldLinear_StartTimed(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, s32 duration, u8 stop,
                           FieldLinearController *movement)
{
    fx32 dx, dy;
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (relative == 1) {
        x += entity->position_x;
        y += entity->position_y;
    }
    movement->axes.x = 1;
    movement->axes.y = 1;
    dx = x - entity->position_x;
    dy = y - entity->position_y;
    movement->bits.paused = 0;
    movement->bits.fixed_duration = 1;
    movement->bits.stop_at_destination = stop;
    movement->bits.braking = 0;
    movement->target = 0;
    movement->elapsed_frames = 0;
    movement->bits.stop_mask_a = 0;
    movement->bits.stop_mask_b = 0;
    movement->start_x = entity->position_x;
    movement->start_y = entity->position_y;
    movement->destination_x = x;
    movement->destination_y = y;
    movement->direction = FX_Atan2Idx(dy, dx) + 0x4000;
    movement->timing.duration_frames = duration;
    movement->velocity_x = dx / duration;
    movement->velocity_y = dy / duration;
    return 1;
}
int FieldLinear_StartFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y,
                               fx32 speed, u8 stop, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    movement->axes.x = entity->position_x != target->position_x + x;
    movement->axes.y = entity->position_y != target->position_y + y;
    if (movement->axes.x || movement->axes.y) {
        movement->bits.active_axes = 1;
        movement->bits.paused = 0;
        movement->bits.fixed_duration = 0;
        movement->bits.stop_at_destination = stop;
        movement->bits.braking = 0;
        movement->target = target;
        movement->elapsed_frames = 0;
        movement->bits.stop_mask_a = 0;
        movement->bits.stop_mask_b = 0;
        movement->timing.speed = speed;
        movement->acceleration = 0;
        movement->maximum_speed = speed;
        movement->deceleration = 0;
        movement->destination_x = x;
        movement->destination_y = y;
        return 1;
    }
    return 0;
}
int FieldLinear_StartTimedFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y,
                                    u32 duration, u8 stop, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    movement->axes.x = 1;
    movement->axes.y = 1;
    movement->bits.paused = 0;
    movement->bits.fixed_duration = 1;
    movement->bits.stop_at_destination = stop;
    movement->bits.stop_mask_a = 0;
    movement->bits.stop_mask_b = 0;
    movement->bits.braking = 0;
    movement->target = target;
    movement->elapsed_frames = 0;
    movement->timing.duration_frames = duration;
    movement->destination_x = x;
    movement->destination_y = y;
    return 1;
}
}
#include <game/field_entity_motion.h>
extern "C" {
#define LINEAR_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))

void FieldLinear_UpdatePosition(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    FieldLinearController temporary;
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes && !movement->bits.paused) {
        if (movement->target) {
            fx32 x = movement->target->position_x + movement->destination_x;
            fx32 y = movement->target->position_y + movement->destination_y;
            if (!movement->bits.fixed_duration) {
                FieldLinear_Start(entity, 0, x, y, movement->timing.speed, movement->acceleration,
                                    movement->maximum_speed, movement->deceleration,
                                    movement->bits.stop_at_destination != 0, &temporary);
            } else {
                int remaining = movement->timing.duration_frames - movement->elapsed_frames;
                if (remaining <= 0)
                    remaining = 1;
                FieldLinear_StartTimed(entity, 0, x, y, remaining, movement->bits.stop_at_destination != 0,
                                    &temporary);
            }
            entity->base.update_linear_movement(&temporary);
        } else {
            if (!movement->bits.fixed_duration && movement->bits.braking) {
                movement->timing.speed += movement->deceleration;
                if (movement->timing.speed < -movement->deceleration)
                    movement->timing.speed = -movement->deceleration;
                func_ov000_020be654(movement->destination_x - entity->position_x,
                                    movement->destination_y - entity->position_y, movement->timing.speed,
                                    &movement->velocity_x, &movement->velocity_y);
            }
            fx32 dx = movement->axes.x ? movement->velocity_x : 0;
            fx32 dy = movement->axes.y ? movement->velocity_y : 0;
            FieldEntity2D_AccumulateMotion(entity, dx, dy);
            if (!movement->bits.fixed_duration && !movement->bits.braking && movement->acceleration > 0) {
                if (movement->timing.speed < movement->maximum_speed) {
                    movement->timing.speed += movement->acceleration;
                    if (movement->timing.speed > movement->maximum_speed)
                        movement->timing.speed = movement->maximum_speed;
                    func_ov000_020be654(movement->destination_x - entity->position_x,
                                        movement->destination_y - entity->position_y, movement->timing.speed,
                                        &movement->velocity_x, &movement->velocity_y);
                }
            }
        }
        ++movement->elapsed_frames;
    }
}

int FieldLinear_CheckCompletion(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes && !movement->bits.paused && movement->bits.stop_at_destination) {
        if (movement->target) {
            if (!movement->bits.fixed_duration) {
                fx32 destination_x = movement->target->position_x + movement->destination_x;
                fx32 destination_y;
                if ((entity->previous_position_x < destination_x && entity->position_x < destination_x) ||
                    (entity->previous_position_x > destination_x && entity->position_x > destination_x))
                    return 1;
                entity->position_x = destination_x;
                destination_y = movement->target->position_y + movement->destination_y;
                if ((entity->previous_position_y < destination_y && entity->position_y < destination_y) ||
                    (entity->previous_position_y > destination_y && entity->position_y > destination_y))
                    return 1;
                entity->position_y = destination_y;
                movement->bits.active_axes = 0;
            } else {
                if (movement->elapsed_frames == movement->timing.duration_frames) {
                    entity->position_x = movement->target->position_x + movement->destination_x;
                    entity->position_y = movement->target->position_y + movement->destination_y;
                    movement->bits.active_axes = 0;
                }
            }
        } else if (!movement->bits.fixed_duration) {
            if (movement->axes.x) {
                if ((movement->start_x > movement->destination_x &&
                     entity->position_x <= movement->destination_x) ||
                    (movement->start_x < movement->destination_x &&
                     entity->position_x >= movement->destination_x)) {
                    entity->position_x = movement->destination_x;
                    movement->axes.x = 0;
                }
            }
            if (movement->axes.y) {
                if ((movement->start_y > movement->destination_y &&
                     entity->position_y <= movement->destination_y) ||
                    (movement->start_y < movement->destination_y &&
                     entity->position_y >= movement->destination_y)) {
                    entity->position_y = movement->destination_y;
                    movement->axes.y = 0;
                }
            }
            if (movement->bits.active_axes && movement->deceleration < 0) {
                fx32 speed = movement->timing.speed;
                fx32 stopping_distance = 0;
                fx32 dx, dy;
                do {
                    speed += movement->deceleration;
                    stopping_distance += speed;
                } while (speed > 0);
                dx = movement->destination_x - entity->position_x;
                dy = movement->destination_y - entity->position_y;
                if (stopping_distance >= FX_Sqrt(LINEAR_MUL(dx, dx) + LINEAR_MUL(dy, dy)))
                    movement->flags |= 0x40;
                else
                    movement->flags &= ~0x40;
            }
        } else {
            if (movement->elapsed_frames == movement->timing.duration_frames) {
                entity->position_x = movement->destination_x;
                entity->position_y = movement->destination_y;
                movement->bits.active_axes = 0;
            }
        }
    }
    return movement->bits.active_axes != 0;
}

void FieldLinear_Cancel(FieldRuntimeEntity *entity, FieldLinearController *movement, int snap_to_destination)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes) {
        movement->bits.active_axes = 0;
        if (snap_to_destination) {
            if (movement->target) {
                entity->position_x = movement->target->position_x + movement->destination_x;
                entity->position_y = movement->target->position_y + movement->destination_y;
            } else {
                entity->position_x = movement->destination_x;
                entity->position_y = movement->destination_y;
            }
        }
    }
}

void FieldLinear_Pause(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes)
        movement->bits.paused = 1;
}
void FieldLinear_Resume(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes)
        movement->bits.paused = 0;
}
}
