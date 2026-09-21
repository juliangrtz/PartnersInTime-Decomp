#ifndef PIT_GAME_FIELD_TIME_HOLE_H
#define PIT_GAME_FIELD_TIME_HOLE_H
#include <nitro.h>

/* The four phases hold the initial value, approach the target, hold it,
 * and return. Values occupy five bits; all durations are frame counters. */
typedef struct FieldColorCycle {
    union {
        u32 raw;
        struct {
            u32 initial : 5, target : 5, current : 5, phase : 2, reserved : 7, remaining : 8;
        } bits;
        u8 bytes[4];
    };
    u8 initial_duration, approach_duration, target_duration, return_duration;
} FieldColorCycle;

/* State shared by time-hole entry, departure and the tunnel presentation.
 * Unnamed portions retain their original storage and remain under research. */
typedef struct FieldTimeHoleState {
    union {
        u32 raw;
        struct { u32 active_or_started : 2, other : 30; } activity;
        struct { u32 unknown00 : 14, mask : 4, unknown18 : 14; } participants;
        struct {
            u32 active : 1, started : 1, mode : 2, unknown4 : 3, reverse : 1;
            u32 entity : 6, unknown14 : 5, scrolling : 1, braking : 1, unknown21 : 1,
                unknown22 : 10;
        } flags;
    };
    union {
        u8 unknown04[24];
        struct {
            u8 arrival_directions[4], unknown08[12];
            s8 participant_indices[4];
            u8 unknown18[4];
        };
    };
    s32 entity_x, entity_y;
    union {
        u8 unknown24[60];
        struct {
            u16 participant_angles[4];
            fx32 participant_radii[4];
            u8 unknown3c[36];
        };
    };
    s32 speed, acceleration, deceleration, limit, braking_distance;
    u8 unknown74[176];
} FieldTimeHoleState;
typedef char FieldColorCycle_SizeCheck[sizeof(FieldColorCycle) == 8 ? 1 : -1];
typedef char FieldTimeHoleState_SizeCheck[sizeof(FieldTimeHoleState) == 292 ? 1 : -1];
#endif
