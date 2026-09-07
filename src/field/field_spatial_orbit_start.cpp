extern "C" {
#include <nitro/fx.h>
extern u16 FX_Atan2Idx(fx32, fx32);
}
#include <game/field_orbit.h>
extern "C" {
extern fx32 func_ov000_020be7d0(fx32, fx32, fx32);
extern fx32 func_ov000_020be894(fx32, fx32);
#define ORBIT_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))

int FieldOrbit3D_StartTimedAroundPoint(FieldRuntimeEntity *entity, int relative_center, fx32 offset_x,
                                       fx32 offset_y, fx32 offset_z, int relative_angle, s32 angle,
                                       s32 duration, int plane, int direction, fx32 scale, u32 stop_mask_a,
                                       u32 stop_mask_b, u8 stop_at_destination, FieldOrbitController *orbit)
{
    s32 initial_angle, destination, remaining;
    fx32 delta_x, delta_y;
    if (!orbit)
        orbit = &entity->orbit_controller;
    if (relative_center == 1) {
        offset_x += entity->position_x;
        offset_y += entity->position_y;
        offset_z += entity->position_z;
    }
    switch (plane) {
    case 0:
        delta_x = entity->position_y - offset_y;
        delta_y = -(entity->position_z - offset_z);
        break;
    case 1:
        delta_x = entity->position_x - offset_x;
        delta_y = -(entity->position_z - offset_z);
        break;
    case 2:
        delta_x = entity->position_x - offset_x;
        delta_y = entity->position_y - offset_y;
        break;
    }
    initial_angle = (u16)(FX_Atan2Idx(delta_y, delta_x) + 0x4000);
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
    orbit->bits.plane = plane;
    orbit->bits.stop_at_destination = stop_at_destination;
    orbit->bits.braking = 0;
    orbit->center_entity = 0;
    orbit->elapsed_frames = 0;
    orbit->bits.stop_mask_a = stop_mask_a;
    orbit->bits.stop_mask_b = stop_mask_b;
    orbit->bytes.direction = direction;
    orbit->center_x = offset_x;
    orbit->center_y = offset_y;
    orbit->center_z = offset_z;
    orbit->initial_angle = initial_angle;
    orbit->destination_angle = destination;
    orbit->angle = initial_angle;
    orbit->remaining_angle = remaining;
    orbit->timing.duration_frames = duration;
    orbit->vertical_scale = scale;
    fx32 distance = func_ov000_020be7d0(delta_x, delta_y, 0);
    orbit->radius = FieldOrbit_AdjustRadius(entity, distance, initial_angle, scale);
    orbit->vertical_radius = ORBIT_MUL(orbit->radius, scale);
    orbit->circumference = func_ov000_020be894(orbit->radius, orbit->vertical_radius);
    orbit->speed = ORBIT_MUL(orbit->circumference, remaining) / (duration * 16);
    return 1;
}
}
