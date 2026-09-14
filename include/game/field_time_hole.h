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
        struct {
            u32 active : 1, started : 1, mode : 2, unknown4 : 3, reverse : 1;
            u32 entity : 6, unknown14 : 5, scrolling : 1, braking : 1, unknown21 : 1,
                unknown22 : 10;
        } flags;
    };
    u8 unknown04[24];
    s32 entity_x, entity_y;
    u8 unknown24[60];
    s32 speed, acceleration, deceleration, limit, braking_distance;
    u8 unknown74[176];
} FieldTimeHoleState;
typedef char FieldColorCycle_SizeCheck[sizeof(FieldColorCycle) == 8 ? 1 : -1];
typedef char FieldTimeHoleState_SizeCheck[sizeof(FieldTimeHoleState) == 292 ? 1 : -1];
#endif
