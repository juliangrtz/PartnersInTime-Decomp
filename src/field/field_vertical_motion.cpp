extern "C" {
#include <nitro/fx.h>
}
#include <game/field_entity_motion.h>
extern "C" {

void FieldVertical_Start(FieldRuntimeEntity *entity, fx32 velocity, fx32 gravity, fx32 terminal_velocity)
{
    if (!velocity)
        velocity = -entity->default_gravity;
    else if (velocity == -1)
        velocity = entity->default_vertical_launch_velocity;
    entity->vertical_velocity = velocity;
    if (gravity == -1)
        gravity = entity->default_gravity;
    entity->vertical_gravity = gravity;
    if (terminal_velocity == -1)
        terminal_velocity = entity->terminal_fall_velocity;
    entity->vertical_terminal_velocity = terminal_velocity;
    entity->field_state_flag_bits.vertical_motion_active = 1;
    entity->field_state_flag_bits.vertical_motion_paused = 0;
    entity->falling_frames = 0;
    entity->vertical_start_z = entity->position_z;
}

void FieldVertical_StartToHeight(FieldRuntimeEntity *entity, fx32 height, fx32 gravity,
                                 fx32 terminal_velocity)
{
    fx32 velocity;
    if (!height) {
        velocity = 0;
    } else {
        if (gravity == -1)
            gravity = entity->initial_gravity;
        if (height == -1) {
            velocity = entity->initial_vertical_launch_velocity;
        } else {
            velocity = 0;
            fx32 accumulated_height = 0;
            do {
                velocity += gravity;
                accumulated_height += velocity;
            } while (accumulated_height < height);
            fx32 excess = accumulated_height - height;
            if (excess > 0)
                velocity -= gravity * FX_Div(excess, velocity) / 4096;
        }
    }
    FieldVertical_Start(entity, velocity, gravity, terminal_velocity);
}

void FieldVertical_Update(FieldRuntimeEntity *entity)
{
    if (!entity->field_state_flag_bits.vertical_motion_paused &&
        entity->field_state_flag_bits.vertical_motion_active) {
        entity->relative_height += entity->vertical_velocity;
        entity->position_z += entity->vertical_velocity;
        if (entity->vertical_velocity < 0)
            ++entity->falling_frames;
        if (entity->vertical_velocity <= 0 && entity->relative_height <= 0) {
            entity->position_z -= entity->relative_height;
            entity->relative_height = 0;
            FieldVertical_Stop(entity);
        } else {
            entity->vertical_velocity -= entity->vertical_gravity;
            fx32 terminal_velocity = entity->vertical_terminal_velocity;
            if (terminal_velocity < 0 && entity->vertical_velocity < terminal_velocity)
                entity->vertical_velocity = terminal_velocity;
        }
    }
}

void FieldVertical_Stop(FieldRuntimeEntity *entity)
{
    entity->field_state_flag_bits.vertical_motion_active = 0;
    entity->field_state_flag_bits.unknown_07 = 0;
    entity->field_state_flag_bits.vertical_motion_paused = 0;
    entity->vertical_start_z = -1;
}
}
