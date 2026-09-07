extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
}
#include <game/field_orbit.h>
extern "C" {
extern fx32 func_ov000_020be7d0(fx32, fx32, fx32);
extern fx32 func_ov000_020be894(fx32, fx32);
#define ORBIT_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))

int FieldOrbit_StartAroundPoint(FieldRuntimeEntity *entity, int relative_center, fx32 offset_x, fx32 offset_y,
                                int relative_angle, s32 angle, fx32 speed, fx32 acceleration,
                                fx32 maximum_speed, fx32 deceleration, int direction, fx32 scale,
                                u8 stop_at_destination, FieldOrbitController *orbit)
{
    s32 initial_angle, destination, remaining;
    if (!orbit)
        orbit = &entity->orbit_controller;
    if (relative_center == 1) {
        offset_x += entity->position_x;
        offset_y += entity->position_y;
    }
    initial_angle = (u16)(FX_Atan2Idx(entity->position_y - offset_y, entity->position_x - offset_x) + 0x4000);
    if (!relative_angle) {
        destination = angle & 0xFFFF;
        if (direction > 0)
            remaining = (angle & 0xFFFF) - initial_angle;
        else
            remaining = initial_angle - (angle & 0xFFFF);
        if (remaining < 0)
            remaining += 0x10000;
        remaining += angle & 0xFFFF0000;
    } else {
        remaining = angle;
        destination = initial_angle + angle * direction;
    }
    if ((offset_x != entity->position_x || offset_y != entity->position_y) && remaining) {
        orbit->bits.active = 1;
        orbit->bits.paused = 0;
        orbit->bits.fixed_duration = 0;
        orbit->bits.stop_at_destination = stop_at_destination;
        orbit->bits.braking = 0;
        orbit->center_entity = 0;
        orbit->elapsed_frames = 0;
        orbit->bits.stop_mask_a = 0;
        orbit->bits.stop_mask_b = 0;
        orbit->bytes.direction = direction;
        orbit->center_x = offset_x;
        orbit->center_y = offset_y;
        orbit->initial_angle = initial_angle;
        orbit->destination_angle = destination;
        orbit->angle = initial_angle;
        orbit->remaining_angle = remaining;
        orbit->vertical_scale = scale;
        fx32 distance = func_ov000_020be7d0(entity->position_x - offset_x, entity->position_y - offset_y, 0);
        orbit->radius = FieldOrbit_AdjustRadius(entity, distance, initial_angle, scale);
        orbit->vertical_radius = ORBIT_MUL(orbit->radius, scale);
        orbit->circumference = func_ov000_020be894(orbit->radius, orbit->vertical_radius);
        if (speed) {
            orbit->speed = speed;
            orbit->timing.acceleration = acceleration;
            orbit->maximum_speed = maximum_speed;
            orbit->deceleration = deceleration;
        } else {
            orbit->speed = entity->locomotion.starting_speed;
            orbit->timing.acceleration = entity->locomotion.acceleration;
            orbit->maximum_speed = entity->locomotion.maximum_speed;
            orbit->deceleration = entity->locomotion.deceleration;
        }
        return 1;
    }
    return 0;
}

int FieldOrbit_StartTimedAroundPoint(FieldRuntimeEntity *entity, int relative_center, fx32 offset_x,
                                     fx32 offset_y, int relative_angle, s32 angle, s32 duration,
                                     int direction, fx32 scale, u8 stop_at_destination,
                                     FieldOrbitController *orbit)
{
    s32 initial_angle, destination, remaining;
    if (!orbit)
        orbit = &entity->orbit_controller;
    if (relative_center == 1) {
        offset_x += entity->position_x;
        offset_y += entity->position_y;
    }
    initial_angle = (u16)(FX_Atan2Idx(entity->position_y - offset_y, entity->position_x - offset_x) + 0x4000);
    if (!relative_angle) {
        destination = angle & 0xFFFF;
        if (direction > 0)
            remaining = (angle & 0xFFFF) - initial_angle;
        else
            remaining = initial_angle - (angle & 0xFFFF);
        if (remaining < 0)
            remaining += 0x10000;
        remaining += angle & 0xFFFF0000;
    } else {
        remaining = angle;
        destination = initial_angle + angle * direction;
    }
    orbit->bits.active = 1;
    orbit->bits.paused = 0;
    orbit->bits.fixed_duration = 1;
    orbit->bits.stop_at_destination = stop_at_destination;
    orbit->bits.braking = 0;
    orbit->center_entity = 0;
    orbit->elapsed_frames = 0;
    orbit->bits.stop_mask_a = 0;
    orbit->bits.stop_mask_b = 0;
    orbit->bytes.direction = direction;
    orbit->center_x = offset_x;
    orbit->center_y = offset_y;
    orbit->initial_angle = initial_angle;
    orbit->destination_angle = destination;
    orbit->angle = initial_angle;
    orbit->remaining_angle = remaining;
    orbit->timing.duration_frames = duration;
    orbit->vertical_scale = scale;
    fx32 distance = func_ov000_020be7d0(entity->position_x - offset_x, entity->position_y - offset_y, 0);
    orbit->radius = FieldOrbit_AdjustRadius(entity, distance, initial_angle, scale);
    orbit->vertical_radius = ORBIT_MUL(orbit->radius, scale);
    orbit->circumference = func_ov000_020be894(orbit->radius, orbit->vertical_radius);
    orbit->speed = ORBIT_MUL(orbit->circumference, remaining) / (duration * 16);
    return 1;
}

int FieldOrbit_StartAroundEntity(FieldRuntimeEntity *entity, FieldRuntimeEntity *center, fx32 offset_x,
                                 fx32 offset_y, int relative_angle, s32 angle, s32 speed, int direction,
                                 fx32 scale, u8 stop_at_destination, FieldOrbitController *orbit)
{
    s32 initial_angle, destination, remaining;
    if (!orbit)
        orbit = &entity->orbit_controller;
    initial_angle =
        (u16)(FX_Atan2Idx(entity->position_y - center->position_y, entity->position_x - center->position_x) +
              0x4000);
    if (!relative_angle) {
        destination = angle & 0xFFFF;
        if (direction > 0)
            remaining = (angle & 0xFFFF) - initial_angle;
        else
            remaining = initial_angle - (angle & 0xFFFF);
        if (remaining < 0)
            remaining += 0x10000;
        remaining += angle & 0xFFFF0000;
    } else {
        remaining = angle;
        destination = initial_angle + angle * direction;
    }
    if ((center->position_x != entity->position_x || center->position_y != entity->position_y) && remaining) {
        orbit->bits.active = 1;
        orbit->bits.paused = 0;
        orbit->bits.fixed_duration = 0;
        orbit->bits.stop_at_destination = stop_at_destination;
        orbit->bits.braking = 0;
        orbit->center_entity = center;
        orbit->elapsed_frames = 0;
        orbit->bits.stop_mask_a = 0;
        orbit->bits.stop_mask_b = 0;
        orbit->bytes.direction = direction;
        orbit->speed = speed;
        orbit->timing.acceleration = 0;
        orbit->maximum_speed = speed;
        orbit->deceleration = 0;
        orbit->initial_angle = initial_angle;
        orbit->destination_angle = destination;
        orbit->angle = initial_angle;
        orbit->remaining_angle = remaining;
        orbit->vertical_scale = scale;
        orbit->center_x = offset_x;
        orbit->center_y = offset_y;
        fx32 distance = func_ov000_020be7d0(entity->position_x - center->position_x,
                                            entity->position_y - center->position_y, 0);
        orbit->radius = FieldOrbit_AdjustRadius(entity, distance, initial_angle, scale);
        orbit->vertical_radius = ORBIT_MUL(orbit->radius, scale);
        orbit->circumference = func_ov000_020be894(orbit->radius, orbit->vertical_radius);
        return 1;
    }
    return 0;
}

int FieldOrbit_StartTimedAroundEntity(FieldRuntimeEntity *entity, FieldRuntimeEntity *center, fx32 offset_x,
                                      fx32 offset_y, int relative_angle, s32 angle, s32 duration,
                                      int direction, fx32 scale, u8 stop_at_destination,
                                      FieldOrbitController *orbit)
{
    s32 initial_angle, destination, remaining;
    if (!orbit)
        orbit = &entity->orbit_controller;
    initial_angle =
        (u16)(FX_Atan2Idx(entity->position_y - center->position_y, entity->position_x - center->position_x) +
              0x4000);
    if (!relative_angle) {
        destination = angle & 0xFFFF;
        if (direction > 0)
            remaining = (angle & 0xFFFF) - initial_angle;
        else
            remaining = initial_angle - (angle & 0xFFFF);
        if (remaining < 0)
            remaining += 0x10000;
        remaining += angle & 0xFFFF0000;
    } else {
        remaining = angle;
        destination = initial_angle + angle * direction;
    }
    orbit->bits.active = 1;
    orbit->bits.paused = 0;
    orbit->bits.fixed_duration = 1;
    orbit->bits.stop_at_destination = stop_at_destination;
    orbit->bits.braking = 0;
    orbit->center_entity = center;
    orbit->elapsed_frames = 0;
    orbit->bits.stop_mask_a = 0;
    orbit->bits.stop_mask_b = 0;
    orbit->bytes.direction = direction;
    orbit->initial_angle = initial_angle;
    orbit->destination_angle = destination;
    orbit->angle = initial_angle;
    orbit->remaining_angle = remaining;
    orbit->timing.duration_frames = duration;
    orbit->vertical_scale = scale;
    orbit->center_x = offset_x;
    orbit->center_y = offset_y;
    fx32 distance = func_ov000_020be7d0(entity->position_x - center->position_x,
                                        entity->position_y - center->position_y, 0);
    orbit->radius = FieldOrbit_AdjustRadius(entity, distance, initial_angle, scale);
    orbit->vertical_radius = ORBIT_MUL(orbit->radius, scale);
    orbit->circumference = func_ov000_020be894(orbit->radius, orbit->vertical_radius);
    orbit->speed = ORBIT_MUL(orbit->circumference, remaining) / (duration * 16);
    return 1;
}
}
