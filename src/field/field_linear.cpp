extern "C" {
#include <nitro/fx.h>
}
#include <game/field_linear.h>
extern "C" {
#define LINEAR_DEFAULT(entity) (&(entity)->linear_controller)
#define LINEAR_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))

extern void func_ov000_020a6394(FieldRuntimeEntity *, int, fx32, fx32, int, int, FieldLinearController *);
extern void func_ov000_020a64a0(FieldRuntimeEntity *, int, fx32, fx32, fx32, fx32, fx32, fx32, int,
                                FieldLinearController *);
extern void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);
extern void func_ov000_020a681c(FieldRuntimeEntity *, fx32, fx32);
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
                func_ov000_020a64a0(entity, 0, x, y, movement->timing.speed, movement->acceleration,
                                    movement->maximum_speed, movement->deceleration,
                                    movement->bits.stop_at_destination != 0, &temporary);
            } else {
                int remaining = movement->timing.duration_frames - movement->elapsed_frames;
                if (remaining <= 0)
                    remaining = 1;
                func_ov000_020a6394(entity, 0, x, y, remaining, movement->bits.stop_at_destination != 0,
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
            func_ov000_020a681c(entity, dx, dy);
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
