#ifndef PIT_GAME_FIELD_PARTY_TRAIL_H
#define PIT_GAME_FIELD_PARTY_TRAIL_H
#include <nitro.h>
struct FieldPartyController;
struct FieldPartyEntity;
/* Each facing direction supplies animation, pixel position and render-order offsets. */
typedef struct FieldPartyTrailDirection {
    s16 animation, x, y, order_offset;
} FieldPartyTrailDirection;
typedef struct FieldPartyTrailHistory {
    struct {
        u32 count : 4, unknown_04_31 : 28;
    } flags;
    u8 unknown_004[1536];
} FieldPartyTrailHistory;
typedef struct FieldPartyTrailEmitter {
    struct {
        u32 unknown_00 : 1, start : 3, next : 3, count : 3, unknown_10_31 : 22;
    } flags;
    u8 timer, interval, unknown_06, animation_offset;
    const FieldPartyTrailDirection *directions;
} FieldPartyTrailEmitter;
typedef char FieldPartyTrailDirection_SizeCheck[sizeof(FieldPartyTrailDirection) == 8 ? 1 : -1];
typedef char FieldPartyTrailHistory_SizeCheck[sizeof(FieldPartyTrailHistory) == 1540 ? 1 : -1];
typedef char FieldPartyTrailEmitter_SizeCheck[sizeof(FieldPartyTrailEmitter) == 12 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void FieldPartyTrail_StartEmitter(struct FieldPartyController *party, struct FieldPartyEntity *owner,
                                  int resource, int animation_offset, int start, int count, u8 interval);
void FieldPartyTrail_UpdateEmitter(struct FieldPartyController *party, struct FieldPartyEntity *owner,
                                   const FieldPartyTrailDirection *directions, s16 animation);
int FieldPartyTrail_HasActiveCopies(struct FieldPartyController *party, struct FieldPartyEntity *owner);
#ifdef __cplusplus
}
#endif
#endif
