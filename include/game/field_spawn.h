#ifndef PIT_GAME_FIELD_SPAWN_H
#define PIT_GAME_FIELD_SPAWN_H

#include <nitro.h>

/* Room placement: eight-pixel cells plus signed pixel offsets.
 * Animation bit 15 selects an initial facing direction instead of an id. */
typedef struct FieldSpawnRecord {
    s16 cell_x, cell_y, cell_z;
    s16 offset_x, offset_y, offset_z;
    u16 animation;
    union {
        u16 raw;
        struct { u16 resource_set : 1, resource_index : 7, presentation_state : 8; } bits;
        struct { u8 resource, presentation_state; } bytes;
    } appearance;
    s16 unknown_10;
    s16 unknown_12;
    u16 unknown_14, unknown_16;
    union {
        u32 raw;
        struct {
            u32 animation_enabled : 1, entity_enabled : 1, contact_mode : 3;
            u32 turn_to_interactor : 1, collision_flag_00 : 1, collision_flags_02_03 : 1;
            u32 collision_flag_01 : 1, collision_flag_06 : 1, collision_flag_04 : 1;
            u32 ignore_navigation_obstacle : 1, synchronize : 1, auto_priorities : 1;
            u32 alternate_collision_faces : 1, track_ground : 1, shadow_style : 3;
            u32 retain_offscreen_contact : 1, subtype : 4, collision_mode : 2, unknown_26_31 : 6;
        } bits;
    } flags;
} FieldSpawnRecord;
typedef char FieldSpawnRecord_SizeCheck[sizeof(FieldSpawnRecord) == 28 ? 1 : -1];

#endif
