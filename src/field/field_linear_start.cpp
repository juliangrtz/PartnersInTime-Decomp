extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
}
#include <game/field_linear.h>
extern "C" {
extern void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);
extern void func_ov000_020a401c(FieldRuntimeEntity *, int);
extern void func_ov000_020a3e38(FieldRuntimeEntity *, int);
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
