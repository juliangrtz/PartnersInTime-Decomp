extern "C" {

/*
 * Roaming options (overlay 0, 0x020B1B88-0x020B2044).
 *
 * The destinations a roaming entity may pick: the bounds it stays inside, the
 * option table, and the weighted choice of the next destination.
 */

#include <game/field_roaming.h>
void FieldRoaming_SetBounds(FieldRuntimeEntity *entity, int minimum_x, int minimum_y, int maximum_x,
                            int maximum_y) {
    entity->unknown_3ec.roaming.bounds[0] = minimum_x;
    entity->unknown_3ec.roaming.bounds[1] = minimum_y;
    entity->unknown_3ec.roaming.bounds[2] = maximum_x;
    entity->unknown_3ec.roaming.bounds[3] = maximum_y;
    entity->roaming_flags |= 2;
}

int FieldRoaming_AddOption(FieldRuntimeEntity *entity, const FieldRoamingOptionInput *input,
                           int index) {
    int end;
    FieldRoamingOption *cursor;
    if (index == -1)
        end = 3;
    else
        end = index;
    if (index == -1)
        index = 0;
    if (index <= end) {
        cursor = &entity->unknown_3ec.roaming.options[index];
        do {
            if (!cursor->flags.enabled) {
                entity->unknown_3ec.roaming.options[index].flags.enabled = 1;
                entity->unknown_3ec.roaming.options[index].unknown_02 = input->argument;
                entity->unknown_3ec.roaming.options[index].flags.delay = (u16)input->delay;
                entity->unknown_3ec.roaming.options[index].flags.direction_mode =
                    (u16)input->direction_mode;
                entity->unknown_3ec.roaming.options[index].speed = input->speed;
                entity->unknown_3ec.roaming.options[index].distance = input->distance << 12;
                entity->roaming_options.count++;
                return index;
            }
            ++index;
            ++cursor;
        } while (index <= end);
    }
    return -1;
}

void FieldRoaming_ClearOptions(FieldRuntimeEntity *entity, int index) {
    int end;
    FieldRoamingOption *cursor;
    if (index == -1)
        end = 3;
    else
        end = index;
    if (index == -1)
        index = 0;
    if (index <= end) {
        cursor = &entity->unknown_3ec.roaming.options[index];
        do {
            if (cursor->flags.enabled) {
                cursor->flags.enabled = 0;
                entity->roaming_options.count--;
            }
            ++index;
            ++cursor;
        } while (index <= end);
    }
    if (!entity->roaming_options.count)
        FieldRoaming_Stop(entity);
}

}

extern "C" {
#include <game/random.h>
}
#include <game/field_roaming.h>
#include <game/field_linear.h>
#include <game/field_geometry.h>
extern "C" void FieldRoaming_ChooseDestination(FieldRuntimeEntity *entity) {
    int remaining = 8;
    FieldRoamingOption *option;
    fx32 x, y, dx, dy;
    do {
        entity->roaming_options.index = Random_NextModulo(entity->roaming_options.count);
        option = &entity->unknown_3ec.roaming.options[entity->roaming_options.index];
        u32 direction;
        if (option->flags.direction_mode == 4)
            direction = (Random_Next() & 3) * 2;
        else
            direction = Random_Next() & 7;
        FieldGeometry_GetDirectionVector(direction, option->distance, &dx, &dy);
        x = entity->position_x + dx;
        y = entity->position_y + dy;
        if (direction == 0 || direction == 1 || direction == 7) {
            int bound = entity->unknown_3ec.roaming.bounds[1] << 12;
            int edge = entity->navigation_min_y;
            if (y + edge < bound) {
                if (!entity->roaming_flag_bits.clamp_to_boundary)
                    continue;
                int clipped = bound - edge;
                if (direction & 1) {
                    int delta = clipped - y;
                    if (direction == 1)
                        x -= delta;
                    else
                        x += delta;
                }
                y = clipped;
            }
        } else if (direction - 3 <= 2) {
            int bound = entity->unknown_3ec.roaming.bounds[3] << 12;
            int edge = entity->navigation_max_y;
            if (y + edge > bound) {
                if (!entity->roaming_flag_bits.clamp_to_boundary)
                    continue;
                int clipped = bound - edge;
                if (direction & 1) {
                    int delta = y - clipped;
                    if (direction == 3)
                        x -= delta;
                    else
                        x += delta;
                }
                y = clipped;
            }
        }
        if (direction - 5 <= 2) {
            int bound = entity->unknown_3ec.roaming.bounds[0] << 12;
            int edge = entity->navigation_min_x;
            if (x + edge < bound) {
                if (!entity->roaming_flag_bits.clamp_to_boundary)
                    continue;
                int clipped = bound - edge;
                if (direction & 1) {
                    int delta = clipped - x;
                    if (direction == 7)
                        y += delta;
                    else
                        y -= delta;
                }
                x = clipped;
            }
        } else if (direction - 1 <= 2) {
            int bound = entity->unknown_3ec.roaming.bounds[2] << 12;
            int edge = entity->navigation_max_x;
            if (x + edge > bound) {
                if (!entity->roaming_flag_bits.clamp_to_boundary)
                    continue;
                int clipped = bound - edge;
                if (direction & 1) {
                    int delta = x - clipped;
                    if (direction == 1)
                        y += delta;
                    else
                        y -= delta;
                }
                x = clipped;
            }
        }
        if (x != entity->position_x || y != entity->position_y)
            break;
    } while (--remaining);
    if (remaining > 0) {
        FieldLinear3D_Start(entity, 0, x, y, entity->position_z, option->speed, 0, option->speed,
                            -option->speed, entity->roaming_flag_bits.unknown_08_13,
                            entity->roaming_flag_bits.unknown_14_19, 1, 0);
        entity->roaming_state.active = 1;
        entity->roaming_state.moving = 1;
        entity->roaming_state.paused = 0;
        entity->roaming_state.path_mode = 0;
    }
}
