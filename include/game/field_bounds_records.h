#ifndef PIT_GAME_FIELD_BOUNDS_RECORDS_H
#define PIT_GAME_FIELD_BOUNDS_RECORDS_H
/* Packed room records retain unknown coordinate and flag roles until their
 * consumers are reconstructed. Coordinates expand from integers to 20.12. */
#include <nitro.h>
#include <nitro/fx.h>
typedef struct FieldBoundaryRecord {
    struct {
        u32 last : 1, index : 10, enabled : 5, modes : 10, unknown_26_31 : 6;
    } flags;
    s16 coordinates[5];
    s16 planes[4];
    u16 reserved;
} FieldBoundaryRecord;
typedef char FieldBoundaryRecord_SizeCheck[sizeof(FieldBoundaryRecord) == 24 ? 1 : -1];
typedef struct FieldBoundaryPlane {
    u32 enabled : 1, mode : 2;
    s32 value : 29;
} FieldBoundaryPlane;
typedef char FieldBoundaryPlane_SizeCheck[sizeof(FieldBoundaryPlane) == 4 ? 1 : -1];
typedef struct FieldBoundaryRuntime {
    struct {
        u32 active : 1, last : 1, index : 10, enabled : 1, mode : 2, unknown_15_31 : 17;
    } flags;
    fx32 coordinates[5];
    FieldBoundaryPlane planes[4];
} FieldBoundaryRuntime;
typedef char FieldBoundaryRuntime_SizeCheck[sizeof(FieldBoundaryRuntime) == 40 ? 1 : -1];
typedef struct FieldPairedBoundsRecord {
    struct {
        u16 last : 1, unknown_01 : 1, mode : 2, unknown_04 : 1, unknown_05_06 : 2, reserved : 9;
    } flags;
    s16 id;
    struct {
        s16 x, y, z, width, height;
    } bounds[2];
} FieldPairedBoundsRecord;
typedef char FieldPairedBoundsRecord_SizeCheck[sizeof(FieldPairedBoundsRecord) == 24 ? 1 : -1];
typedef struct FieldPairedBoundsRuntime {
    struct {
        u16 active : 1, last : 1, unknown_02 : 1, mode : 3, unknown_06 : 1, unknown_07_08 : 2, reserved : 7;
    } flags;
    s16 id;
    struct {
        fx32 min_x, max_x, min_y, max_y, z;
    } bounds[2];
} FieldPairedBoundsRuntime;
typedef char FieldPairedBoundsRuntime_SizeCheck[sizeof(FieldPairedBoundsRuntime) == 44 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void FieldArea_ExpandPairedBounds(void *unused, const FieldPairedBoundsRecord *record,
                                  FieldPairedBoundsRuntime *bounds);
void FieldArea_ExpandBoundaryRecords(void *unused, const FieldBoundaryRecord *record,
                                     FieldBoundaryRuntime *bounds);
#ifdef __cplusplus
}
#endif
#endif
