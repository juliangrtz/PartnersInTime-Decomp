/*
 * Time hole state (overlay 0, 0x02066C50-0x02066C68).
 *
 * Whether a time hole is currently active.
 */

#include <game/field_system.h>
int FieldSystem_IsTimeHoleActive(FieldSystem *system) {
    return system->time_hole.activity.active_or_started != 0;
}
