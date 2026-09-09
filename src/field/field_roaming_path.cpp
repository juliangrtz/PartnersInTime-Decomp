#include <game/field_roaming.h>
#include <game/field_linear.h>
extern "C" {
u32 Random_Next(void);
void func_ov000_020b1b88(FieldRuntimeEntity *entity);
void FieldPath_Clear(FieldRuntimeEntity *entity)
{
    entity->unknown_3ec.path.flags.enabled = 0;
    FieldRoaming_Stop(entity);
}
void FieldPath_Start(FieldRuntimeEntity *entity)
{
    if (entity->unknown_3ec.path.flags.enabled) {
        FieldPathPoint *point = entity->unknown_3ec.path.points;
        int remaining;
        int x, y;
        entity->path_state.index = 0;
        entity->path_state.backward = 0;
        remaining = entity->path_state.count;
        while (remaining) {
            x = point->x;
            y = point->y;
            if (entity->position_x != x || entity->position_y != y)
                break;
            --remaining;
            ++point;
        }
        if (remaining > 0) {
            FieldLinear3D_Start(entity, 0, x, y, entity->position_z, 0, 0, 0, 0,
                                entity->path_state.stop_mask_a, entity->path_state.stop_mask_b, 1, 0);
            entity->roaming_state.active = 1;
            entity->roaming_state.moving = 1;
            entity->roaming_state.paused = 0;
            entity->roaming_state.path_mode = 1;
        }
    }
}
void FieldPath_Advance(FieldRuntimeEntity *entity)
{
    /* The native path stores interleaved X/Y coordinates. */
    fx32 *points = entity->unknown_3ec.path.coordinates;
    int backward, index;
    if (entity->unknown_3ec.path.flags.random_direction)
        backward = Random_Next() & 1;
    else
        backward = entity->path_state.backward;
    if (!backward) {
        u32 count;
        index = entity->path_state.index + 1;
        count = entity->path_state.count;
        if (index >= count) {
            if (!entity->unknown_3ec.path.flags.bounce)
                index = 0;
            else {
                entity->path_state.backward = 1;
                index = count - 2;
            }
        }
    } else {
        index = entity->path_state.index - 1;
        if (index < 0) {
            if (!entity->unknown_3ec.path.flags.bounce)
                index = entity->path_state.count - 1;
            else {
                entity->path_state.backward = 0;
                index = 1;
            }
        }
    }

    entity->path_state.index = index;
    FieldLinear3D_Start(entity, 0, points[2 * index], points[2 * index + 1], entity->position_z, 0, 0, 0, 0,
                        entity->path_state.stop_mask_a, entity->path_state.stop_mask_b, 1, 0);
    entity->roaming_state.moving = 1;
}
void FieldRoaming_Stop(FieldRuntimeEntity *entity)
{
    if (entity->roaming_state.active) {
        entity->roaming_state.active = 0;
        if (entity->roaming_state.moving) {
            entity->base.cancel_linear_movement(0, 0);
            entity->roaming_state.moving = 0;
        }
    }
}
void FieldRoaming_Pause(FieldRuntimeEntity *entity)
{
    if (entity->roaming_state.active) {
        entity->roaming_state.paused = 1;
        if (entity->roaming_state.moving)
            FieldLinear_Pause(entity, 0);
    }
}
void FieldRoaming_Resume(FieldRuntimeEntity *entity)
{
    if (entity->roaming_state.active) {
        entity->roaming_state.paused = 0;
        if (entity->roaming_state.moving)
            FieldLinear_Resume(entity, 0);
    }
}
void FieldRoaming_Update(FieldRuntimeEntity *entity)
{
    if (entity->roaming_state.active && !entity->roaming_state.paused) {
        if (entity->roaming_state.moving) {
            if (entity->linear_controller.bits.active_axes)
                return;
            if (!entity->roaming_state.path_mode) {
                int delay = entity->unknown_3ec.roaming.options[entity->roaming_options.index].flags.delay;
                if (delay > 0) {
                    entity->unknown_3d8 = delay;
                    entity->roaming_state.moving = 0;
                    return;
                }
                func_ov000_020b1b88(entity);
                return;
            }
            if (entity->unknown_3ec.path.flags.delay) {
                entity->unknown_3d8 = entity->unknown_3ec.path.flags.delay;
                entity->roaming_state.moving = 0;
                return;
            }
            FieldPath_Advance(entity);
            return;
        }
        if (entity->unknown_3d8)
            entity->unknown_3d8--;
        if (!entity->unknown_3d8) {
            if (!entity->roaming_state.path_mode)
                func_ov000_020b1b88(entity);
            else
                FieldPath_Advance(entity);
        }
    }
}
}
