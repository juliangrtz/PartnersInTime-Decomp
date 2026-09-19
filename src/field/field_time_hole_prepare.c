/*
 * Time hole arrival (overlay 0, 0x02069AAC-0x02069B24).
 *
 * Prepares the entity that arrives out of a time hole and starts its arrival.
 */

#include <game/field_system.h>
extern void func_0202cbd4(void *, int, u32);
void FieldSystem_PrepareTimeHoleEntity(FieldSystem *system, FieldRuntimeEntity *entity) {
    func_0202cbd4(&system->time_hole, 0, 292);
    system->time_hole.flags.active = 1;
    system->time_hole.flags.entity = entity->base.index;
    system->time_hole.entity_x = entity->position_x;
    system->time_hole.entity_y = entity->position_y - 0x10000;
}

void FieldSystem_StartPreparedTimeHoleArrival(FieldSystem *system) {
    system->time_hole.flags.started = 1;
}
