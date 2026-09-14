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
