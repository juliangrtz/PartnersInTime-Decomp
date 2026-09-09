extern "C" {
#include <nitro/fx.h>
}
#include <game/field_entity_motion.h>
#include <game/field_linear.h>
#include <game/field_geometry.h>
extern "C" {
#define LINEAR_DEFAULT(entity) (&(entity)->linear_controller)
extern void func_ov000_020be654(fx32, fx32, fx32, fx32 *, fx32 *);

void FieldLinear3D_UpdatePosition(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    FieldLinearController temporary;
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes && !movement->bits.paused) {
        FieldRuntimeEntity *target = movement->target;
        if (target) {
            fx32 x = target->position_x + movement->destination_x;
            fx32 y = target->position_y + movement->destination_y;
            fx32 z = target->position_z + movement->destination_z;
            if (!movement->bits.fixed_duration) {
                FieldLinear3D_Start(entity, 0, x, y, z, movement->timing.speed, movement->acceleration,
                                    movement->maximum_speed, movement->deceleration,
                                    movement->bits.stop_mask_a, movement->bits.stop_mask_b,
                                    movement->bits.stop_at_destination != 0, &temporary);
            } else {
                int remaining = movement->timing.duration_frames - movement->elapsed_frames;
                if (remaining <= 0)
                    remaining = 1;
                FieldLinear3D_StartTimed(entity, 0, x, y, z, remaining, movement->bits.stop_mask_a,
                                    movement->bits.stop_mask_b, movement->bits.stop_at_destination != 0,
                                    &temporary);
            }
            entity->base.update_linear_movement(&temporary);
        } else {
            if (!movement->bits.fixed_duration && movement->bits.braking) {
                movement->timing.speed += movement->deceleration;
                if (movement->timing.speed < -movement->deceleration)
                    movement->timing.speed = -movement->deceleration;
                fx32 dx = movement->axes.x ? movement->destination_x - entity->position_x : 0;
                fx32 dy = movement->axes.y ? movement->destination_y - entity->position_y : 0;
                fx32 dz = movement->axes.z ? movement->destination_z - entity->position_z : 0;
                if (entity->field_state_flag_bits.track_ground)
                    func_ov000_020be654(dx, dy, movement->timing.speed, &movement->velocity_x,
                                        &movement->velocity_y);
                else
                    FieldGeometry_ScaleVector(dx, dy, dz, movement->timing.speed, &movement->velocity_x,
                                        &movement->velocity_y, &movement->velocity_z);
            }
            fx32 dx = movement->axes.x ? movement->velocity_x : 0;
            fx32 dy = movement->axes.y ? movement->velocity_y : 0;
            fx32 dz = movement->axes.z ? movement->velocity_z : 0;
            FieldEntity3D_AccumulateMotion(entity, dx, dy, dz);
            if (!movement->bits.fixed_duration && !movement->bits.braking && movement->acceleration > 0) {
                if (movement->timing.speed < movement->maximum_speed) {
                    movement->timing.speed += movement->acceleration;
                    if (movement->timing.speed > movement->maximum_speed)
                        movement->timing.speed = movement->maximum_speed;
                    if (entity->field_state_flag_bits.track_ground) {
                        func_ov000_020be654(movement->destination_x - entity->position_x,
                                            movement->destination_y - entity->position_y,
                                            movement->timing.speed, &movement->velocity_x,
                                            &movement->velocity_y);
                    } else {
                        FieldGeometry_ScaleVector(movement->destination_x - entity->position_x,
                                            movement->destination_y - entity->position_y,
                                            movement->destination_z - entity->position_z,
                                            movement->timing.speed, &movement->velocity_x,
                                            &movement->velocity_y, &movement->velocity_z);
                    }
                }
            }
        }
        ++movement->elapsed_frames;
    }
}

int FieldLinear3D_CheckCompletion(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes && !movement->bits.paused && movement->bits.stop_at_destination) {
        FieldRuntimeEntity *target = movement->target;
        if (target) {
            if (!movement->bits.fixed_duration) {
                fx32 destination_x = target->position_x + movement->destination_x;
                fx32 destination_y;
                if ((entity->previous_position_x < destination_x && entity->position_x < destination_x) ||
                    (entity->previous_position_x > destination_x && entity->position_x > destination_x))
                    return 1;
                entity->position_x = destination_x;
                destination_y = target->position_y + movement->destination_y;
                if ((entity->previous_position_y < destination_y && entity->position_y < destination_y) ||
                    (entity->previous_position_y > destination_y && entity->position_y > destination_y))
                    return 1;
                entity->position_y = destination_y;
                if (!entity->field_state_flag_bits.track_ground) {
                    fx32 destination_z = target->position_z + movement->destination_z;
                    if ((entity->previous_position_z < destination_z && entity->position_z < destination_z) ||
                        (entity->previous_position_z > destination_z && entity->position_z > destination_z))
                        return 1;
                    entity->position_z = destination_z;
                }
                movement->bits.active_axes = 0;
            } else {
                if (movement->elapsed_frames == movement->timing.duration_frames) {
                    entity->position_x = target->position_x + movement->destination_x;
                    entity->position_y = target->position_y + movement->destination_y;
                    if (!entity->field_state_flag_bits.track_ground)
                        entity->position_z = target->position_z + movement->destination_z;
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
            if (movement->axes.z) {
                if ((movement->start_z > movement->destination_z &&
                     entity->position_z <= movement->destination_z) ||
                    (movement->start_z < movement->destination_z &&
                     entity->position_z >= movement->destination_z)) {
                    entity->position_z = movement->destination_z;
                    movement->axes.z = 0;
                }
            }
            if (movement->bits.active_axes && movement->deceleration < 0) {
                fx32 speed = movement->timing.speed;
                fx32 stopping_distance = 0;
                fx32 dx, dy, dz, distance;
                do {
                    speed += movement->deceleration;
                    stopping_distance += speed;
                } while (speed > 0);
                dx = movement->axes.x ? movement->destination_x - entity->position_x : 0;
                dy = movement->axes.y ? movement->destination_y - entity->position_y : 0;
                dz = movement->axes.z ? movement->destination_z - entity->position_z : 0;
                if (entity->field_state_flag_bits.track_ground)
                    distance = FieldGeometry_GetVectorLength(dx, dy, 0);
                else
                    distance = FieldGeometry_GetVectorLength(dx, dy, dz);
                if (stopping_distance >= distance)
                    movement->flags |= 0x40;
                else
                    movement->flags &= ~0x40;
            }
        } else {
            if (movement->elapsed_frames == movement->timing.duration_frames) {
                entity->position_x = movement->destination_x;
                entity->position_y = movement->destination_y;
                if (!entity->field_state_flag_bits.track_ground)
                    entity->position_z = movement->destination_z;
                movement->bits.active_axes = 0;
            }
        }
    }
    return movement->bits.active_axes != 0;
}

int FieldLinear3D_CheckStopMasks(FieldRuntimeEntity *entity, FieldLinearController *movement)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if ((movement->bits.stop_mask_a & entity->unknown_3a0_bits.contact_mask_a) ||
        (movement->bits.stop_mask_b & entity->runtime_flag_bits.contact_mask_b)) {
        movement->bits.active_axes = 0;
        return 0;
    }
    return 1;
}

void FieldLinear3D_Cancel(FieldRuntimeEntity *entity, FieldLinearController *movement,
                          int snap_to_destination)
{
    if (!movement)
        movement = LINEAR_DEFAULT(entity);
    if (movement->bits.active_axes) {
        movement->bits.active_axes = 0;
        if (snap_to_destination) {
            FieldRuntimeEntity *target = movement->target;
            if (target) {
                entity->position_x = target->position_x + movement->destination_x;
                entity->position_y = target->position_y + movement->destination_y;
                if (!entity->field_state_flag_bits.track_ground)
                    entity->position_z = target->position_z + movement->destination_z;
            } else {
                entity->position_x = movement->destination_x;
                entity->position_y = movement->destination_y;
                if (!entity->field_state_flag_bits.track_ground)
                    entity->position_z = movement->destination_z;
            }
        }
    }
}
}
