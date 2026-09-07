extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
}
#include <game/field_linear.h>
extern "C" {
extern void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);
extern void func_ov000_020be4f0(fx32, fx32, fx32, fx32, fx32 *, fx32 *, fx32 *);
#define LINEAR_DEFAULT(entity) (&(entity)->linear_controller)

int FieldLinear3D_Start(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, fx32 z, fx32 speed,
                        fx32 acceleration, fx32 maximum_speed, fx32 deceleration, u32 stop_mask_a,
                        u32 stop_mask_b, u8 stop, FieldLinearController *movement)
{
    fx32 dx, dy, dz;
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (relative == 1) {
        x += entity->position_x;
        y += entity->position_y;
        z += entity->position_z;
    }
    movement->axes.x = x != entity->position_x;
    movement->axes.y = y != entity->position_y;
    movement->axes.z = !entity->field_state_flag_bits.track_ground && z != entity->position_z;
    if (movement->axes.x || movement->axes.y || movement->axes.z) {
        dx = x - entity->position_x;
        dy = y - entity->position_y;
        dz = z - entity->position_z;
        movement->bits.paused = 0;
        movement->bits.fixed_duration = 0;
        movement->bits.stop_at_destination = stop;
        movement->bits.braking = 0;
        movement->target = 0;
        movement->elapsed_frames = 0;
        movement->bits.stop_mask_a = stop_mask_a;
        movement->bits.stop_mask_b = stop_mask_b;
        movement->start_x = entity->position_x;
        movement->start_y = entity->position_y;
        movement->start_z = entity->position_z;
        movement->destination_x = x;
        movement->destination_y = y;
        movement->destination_z = z;
        movement->direction = FX_Atan2Idx(dy, dx) + 0x4000;
        if (!entity->field_state_flag_bits.track_ground) {
            fx32 square_x = ((s64)dx * dx + 0x800) >> 12;
            fx32 square_y = ((s64)dy * dy + 0x800) >> 12;
            movement->vertical_direction = FX_Atan2Idx(FX_Sqrt(square_x + square_y), dz) + 0x4000;
        }
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
        if (entity->field_state_flag_bits.track_ground)
            func_ov000_020be654(dx, dy, speed, &movement->velocity_x, &movement->velocity_y);
        else
            func_ov000_020be4f0(dx, dy, dz, speed, &movement->velocity_x, &movement->velocity_y,
                                &movement->velocity_z);
        if (movement->axes.x && !movement->velocity_x)
            movement->velocity_x = dx;
        if (movement->axes.y && !movement->velocity_y)
            movement->velocity_y = dy;
        if (movement->axes.z && !movement->velocity_z)
            movement->velocity_z = dz;
        return 1;
    }
    return 0;
}
int FieldLinear3D_StartTimed(FieldRuntimeEntity *entity, int relative, fx32 x, fx32 y, fx32 z, s32 duration,
                             u32 stop_mask_a, u32 stop_mask_b, u8 stop, FieldLinearController *movement)
{
    fx32 dx, dy, dz;
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (relative == 1) {
        x += entity->position_x;
        y += entity->position_y;
        z += entity->position_z;
    }
    movement->axes.x = x != entity->position_x;
    movement->axes.y = y != entity->position_y;
    movement->axes.z = !entity->field_state_flag_bits.track_ground && z != entity->position_z;
    dx = x - entity->position_x;
    dy = y - entity->position_y;
    dz = z - entity->position_z;
    movement->bits.paused = 0;
    movement->bits.fixed_duration = 1;
    movement->bits.stop_at_destination = stop;
    movement->bits.braking = 0;
    movement->target = 0;
    movement->elapsed_frames = 0;
    movement->bits.stop_mask_a = stop_mask_a;
    movement->bits.stop_mask_b = stop_mask_b;
    movement->start_x = entity->position_x;
    movement->start_y = entity->position_y;
    movement->start_z = entity->position_z;
    movement->destination_x = x;
    movement->destination_y = y;
    movement->destination_z = z;
    movement->direction = FX_Atan2Idx(dy, dx) + 0x4000;
    if (!entity->field_state_flag_bits.track_ground) {
        fx32 square_x = ((s64)dx * dx + 0x800) >> 12;
        fx32 square_y = ((s64)dy * dy + 0x800) >> 12;
        movement->vertical_direction = FX_Atan2Idx(FX_Sqrt(square_x + square_y), dz) + 0x4000;
    }
    movement->timing.duration_frames = duration;
    if (movement->axes.x)
        movement->velocity_x = dx / duration;
    if (movement->axes.y)
        movement->velocity_y = dy / duration;
    if (movement->axes.z)
        movement->velocity_z = dz / duration;
    return 1;
}
int FieldLinear3D_StartFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y,
                                 fx32 z, fx32 speed, u32 stop_mask_a, u32 stop_mask_b, u8 stop,
                                 FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    movement->axes.x = entity->position_x != target->position_x + x;
    movement->axes.y = entity->position_y != target->position_y + y;
    movement->axes.z =
        !entity->field_state_flag_bits.track_ground && entity->position_z != target->position_z + z;
    if (movement->axes.x || movement->axes.y || movement->axes.z || !stop) {
        movement->bits.active_axes = 1;
        movement->bits.paused = 0;
        movement->bits.fixed_duration = 0;
        movement->bits.stop_at_destination = stop;
        movement->bits.braking = 0;
        movement->target = target;
        movement->elapsed_frames = 0;
        movement->bits.stop_mask_a = stop_mask_a;
        movement->bits.stop_mask_b = stop_mask_b;
        movement->timing.speed = speed;
        movement->acceleration = 0;
        movement->maximum_speed = speed;
        movement->deceleration = 0;
        movement->destination_x = x;
        movement->destination_y = y;
        movement->destination_z = z;
        return 1;
    }
    return 0;
}
int FieldLinear3D_StartTimedFollowing(FieldRuntimeEntity *entity, FieldRuntimeEntity *target, fx32 x, fx32 y,
                                      fx32 z, u32 duration, u32 stop_mask_a, u32 stop_mask_b, u8 stop,
                                      FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    movement->axes.x = entity->position_x != target->position_x + x;
    movement->axes.y = entity->position_y != target->position_y + y;
    movement->axes.z =
        !entity->field_state_flag_bits.track_ground && entity->position_z != target->position_z + z;
    movement->bits.active_axes = 1;
    movement->bits.paused = 0;
    movement->bits.fixed_duration = 1;
    movement->bits.stop_at_destination = stop;
    movement->bits.stop_mask_a = stop_mask_a;
    movement->bits.stop_mask_b = stop_mask_b;
    movement->bits.braking = 0;
    movement->target = target;
    movement->elapsed_frames = 0;
    movement->timing.duration_frames = duration;
    movement->destination_x = x;
    movement->destination_y = y;
    movement->destination_z = z;
    return 1;
}
}
