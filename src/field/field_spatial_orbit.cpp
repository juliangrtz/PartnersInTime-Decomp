extern "C" {
#include <nitro/fx.h>
}
#include <game/field_entity_motion.h>
#include <game/field_orbit.h>
extern "C" {
extern const s16 FX_SinCosTable_[];
void FieldOrbit3D_CalculatePosition(FieldRuntimeEntity *entity, fx32 *x, fx32 *y, fx32 *z,
                                    FieldOrbitController *orbit);
#define DEFAULT_ORBIT(entity) (&(entity)->orbit_controller)
#define ORBIT_MUL(a, b) ((fx32)(((s64)(a) * (b) + 2048) >> 12))
#define ORBIT_SIN(angle) FX_SinCosTable_[((((angle) - 0x4000) & 0xFFFF) >> 4) * 2]
#define ORBIT_COS(angle) FX_SinCosTable_[((((angle) - 0x4000) & 0xFFFF) >> 4) * 2 + 1]

static inline void SnapSpatialOrbit(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    fx32 x, y, z;
    orbit->angle = orbit->destination_angle;
    FieldOrbit3D_CalculatePosition(entity, &x, &y, &z, orbit);
    entity->position_x = x;
    entity->position_y = y;
    if (!entity->field_state_flag_bits.track_ground)
        entity->position_z = z;
}

void FieldOrbit3D_UpdatePosition(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    fx32 x, y, z;
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
        FieldOrbit3D_CalculatePosition(entity, &x, &y, &z, orbit);
        FieldEntity3D_AccumulateMotion(entity, x - entity->position_x, y - entity->position_y,
                            entity->field_state_flag_bits.track_ground ? 0 : z - entity->position_z);
        if (!orbit->bits.fixed_duration && !orbit->bits.braking && orbit->timing.acceleration > 0) {
            if (orbit->speed < orbit->maximum_speed) {
                orbit->speed += orbit->timing.acceleration;
                if (orbit->speed > orbit->maximum_speed)
                    orbit->speed = orbit->maximum_speed;
            }
        }
    }
}

int FieldOrbit3D_AdvanceFrame(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active && !orbit->bits.paused) {
        if (orbit->bits.stop_at_destination) {
            if (!orbit->bits.fixed_duration) {
                if (orbit->remaining_angle <= 0) {
                    SnapSpatialOrbit(entity, orbit);
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
                    SnapSpatialOrbit(entity, orbit);
                    orbit->bits.active = 0;
                }
            }
        }
        ++orbit->elapsed_frames;
    }
    return orbit->bits.active != 0;
}

int FieldOrbit3D_CheckStopMasks(FieldRuntimeEntity *entity, FieldOrbitController *orbit)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if ((orbit->bits.stop_mask_a & entity->unknown_3a0_bits.contact_mask_a) ||
        (orbit->bits.stop_mask_b & entity->runtime_flag_bits.contact_mask_b)) {
        orbit->bits.active = 0;
        return 0;
    }
    return 1;
}

void FieldOrbit3D_Cancel(FieldRuntimeEntity *entity, FieldOrbitController *orbit, int snap_to_destination)
{
    if (!orbit)
        orbit = DEFAULT_ORBIT(entity);
    if (orbit->bits.active) {
        orbit->bits.active = 0;
        if (snap_to_destination) {
            SnapSpatialOrbit(entity, orbit);
        }
    }
}

void FieldOrbit3D_CalculatePosition(FieldRuntimeEntity *entity, fx32 *x, fx32 *y, fx32 *z,
                                    FieldOrbitController *orbit)
{
    fx32 radial_x = ORBIT_MUL(orbit->radius, ORBIT_COS(orbit->angle));
    fx32 radial_y = ORBIT_MUL(orbit->radius, ORBIT_SIN(orbit->angle));
    radial_y = ORBIT_MUL(radial_y, orbit->vertical_scale);
    FieldRuntimeEntity *center = orbit->center_entity;
    if (center) {
        switch (orbit->bits.plane) {
        case 0:
            *x = entity->position_x + (center->position_x - center->previous_position_x);
            *y = radial_x + (center->position_y + orbit->center_y);
            *z = center->position_z + orbit->center_z - radial_y;
            break;
        case 1:
            *x = radial_x + (center->position_x + orbit->center_x);
            *y = entity->position_y + (center->position_y - center->previous_position_y);
            *z = center->position_z + orbit->center_z - radial_y;
            break;
        case 2:
            *x = radial_x + (center->position_x + orbit->center_x);
            *y = radial_y + (center->position_y + orbit->center_y);
            *z = entity->position_z + (center->position_z - center->previous_position_z);
            break;
        }
    } else {
        switch (orbit->bits.plane) {
        case 0:
            *x = entity->position_x;
            *y = orbit->center_y + radial_x;
            *z = orbit->center_z - radial_y;
            break;
        case 1:
            *x = orbit->center_x + radial_x;
            *y = entity->position_y;
            *z = orbit->center_z - radial_y;
            break;
        case 2:
            *x = orbit->center_x + radial_x;
            *y = orbit->center_y + radial_y;
            *z = entity->position_z;
            break;
        }
    }
}
}
