#include <game/field_system.h>
int FieldSystem_IsTimeHoleActive(FieldSystem *system) {
    return system->time_hole.activity.active_or_started != 0;
}
