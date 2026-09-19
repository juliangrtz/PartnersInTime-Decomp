/*
 * Vertical scroll (overlay 0, 0x02068FAC-0x02069060).
 *
 * The vertical scroll the field runs, with the deceleration that ends it.
 */

#include <game/field_system.h>
void FieldSystem_StartVerticalScroll(FieldSystem *system, int reverse, fx32 speed,
                                     fx32 acceleration, fx32 limit) {
    if (reverse != -1)
        system->time_hole.flags.reverse = reverse;
    system->time_hole.flags.scrolling = 1;
    system->time_hole.flags.braking = 0;
    system->time_hole.flags.unknown21 = 0;
    system->time_hole.speed = speed;
    system->time_hole.acceleration = acceleration;
    system->time_hole.limit = limit;
}

void FieldSystem_DecelerateVerticalScroll(FieldSystem *system, fx32 deceleration) {
    int velocity;
    system->time_hole.flags.braking = 1;
    system->time_hole.deceleration = deceleration;
    system->time_hole.braking_distance = 0;
    velocity = system->time_hole.speed + deceleration;
    while (velocity > 0) {
        system->time_hole.braking_distance += velocity;
        velocity += deceleration;
    }
}

int FieldSystem_IsVerticalScrollActive(FieldSystem *system) {
    return system->time_hole.flags.scrolling != 0;
}
