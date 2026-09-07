extern "C" {
#include <nitro/fx.h>
}
#include <game/field_orbit.h>
extern "C" {
extern const s16 FX_SinCosTable_[];
extern void func_ov000_020a681c(FieldRuntimeEntity *, fx32, fx32);

#define DEFAULT_ORBIT(entity) (&(entity)->orbit_controller)
#define ORBIT_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))
#define ORBIT_SIN(angle) FX_SinCosTable_[((((angle) - 0x4000) & 0xFFFF) >> 4) * 2]
#define ORBIT_COS(angle) FX_SinCosTable_[((((angle) - 0x4000) & 0xFFFF) >> 4) * 2 + 1]

void FieldOrbit_CalculatePosition(FieldRuntimeEntity *entity, fx32 *x, fx32 *y, FieldOrbitController *orbit);

void FieldOrbit_UpdatePosition(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    fx32 x, y;
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active && !orbit->bits.paused) {
        s32 delta;
        if (!orbit->bits.fixed_duration && orbit->bits.braking) {
            orbit->speed += orbit->deceleration;
            if (orbit->speed < -orbit->deceleration)
                orbit->speed = -orbit->deceleration;
        }
        delta = FX_Div(0x10000, FX_Div(orbit->circumference, orbit->speed));
        orbit->angle += delta * orbit->bytes.direction;
        orbit->remaining_angle -= delta;
        FieldOrbit_CalculatePosition(entity, &x, &y, orbit);
        func_ov000_020a681c(entity, x - entity->position_x, y - entity->position_y);
        if (!orbit->bits.fixed_duration && !orbit->bits.braking && orbit->timing.acceleration > 0) {
            if (orbit->speed < orbit->maximum_speed) {
                orbit->speed += orbit->timing.acceleration;
                if (orbit->speed > orbit->maximum_speed)
                    orbit->speed = orbit->maximum_speed;
            }
        }
    }
}

int FieldOrbit_AdvanceFrame(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active && !orbit->bits.paused) {
        if (orbit->bits.stop_at_destination) {
            if (!orbit->bits.fixed_duration) {
                if (orbit->remaining_angle <= 0) {
                    orbit->angle = orbit->destination_angle;
                    FieldOrbit_CalculatePosition(entity, &entity->position_x, &entity->position_y, orbit);
                    orbit->bits.active = 0;
                }
                if (orbit->bits.active && orbit->deceleration < 0) {
                    fx32 speed = orbit->speed;
                    fx32 distance = 0;
                    do {
                        speed += orbit->deceleration;
                        distance += speed;
                    } while (speed > 0);
                    if (FX_Div(0x10000, FX_Div(orbit->circumference, distance)) >= orbit->remaining_angle)
                        orbit->flags |= 0x40;
                    else
                        orbit->flags &= ~0x40;
                }
            } else {
                if (orbit->elapsed_frames == orbit->timing.duration_frames) {
                    orbit->angle = orbit->destination_angle;
                    FieldOrbit_CalculatePosition(entity, &entity->position_x, &entity->position_y, orbit);
                    orbit->bits.active = 0;
                }
            }
        }
        ++orbit->elapsed_frames;
    }
    return orbit->bits.active != 0;
}

void FieldOrbit_Cancel(FieldRuntimeEntity *entity, FieldOrbitController *orbit, int snap_to_destination)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active) {
        orbit->bits.active = 0;
        if (snap_to_destination) {
            orbit->angle = orbit->destination_angle;
            FieldOrbit_CalculatePosition(entity, &entity->position_x, &entity->position_y, orbit);
        }
    }
}

void FieldOrbit_Pause(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active)
        orbit->bits.paused = 1;
}

void FieldOrbit_Resume(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active)
        orbit->bits.paused = 0;
}

void FieldOrbit_CalculatePosition(FieldRuntimeEntity *entity, fx32 *x, fx32 *y, FieldOrbitController *orbit)
{
    fx32 center_x, center_y;
    if (orbit->center_entity) {
        center_x = orbit->center_entity->position_x;
        center_y = orbit->center_entity->position_y;
    } else {
        center_x = orbit->center_x;
        center_y = orbit->center_y;
    }
    *x = center_x + ORBIT_MUL(orbit->radius, ORBIT_COS(orbit->angle));
    fx32 radial_y = ORBIT_MUL(orbit->radius, ORBIT_SIN(orbit->angle));
    *y = center_y + ORBIT_MUL(radial_y, orbit->vertical_scale);
}

fx32 FieldOrbit_AdjustRadius(FieldRuntimeEntity *entity, fx32 radius, u16 angle, fx32 scale)
{
    s32 sine = ORBIT_SIN(angle);
    if (sine < 0)
        sine = -sine;
    fx32 inverse_difference = FX_Inv(scale) - 4096;
    fx32 correction = ORBIT_MUL(inverse_difference, sine);
    return radius + ORBIT_MUL(radius, correction);
}
}
