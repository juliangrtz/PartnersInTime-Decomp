#ifndef PIT_GAME_OVERLAY007_PARTY_H
#define PIT_GAME_OVERLAY007_PARTY_H

#include <nitro.h>
#include <game/text.h>

/* The scene's party roster: the rotating display order, the per-slot values the
   interface shows and the text object the captions are measured with. */
typedef struct Overlay7Party {
    u8 unknown_00;
    u8 kind;
    u8 unknown_02[2];
    u8 slots[128];
    s8 *values;
    u8 unknown_88[128];
    s8 first;
    u8 count;
    u8 unknown_10a;
    union { s8 offset; u8 index; } span;
    /* Per-kind starting offset into the rotating order. */
    s8 offsets[4];
    u8 unknown_110[12];
    GameText text;
} Overlay7Party;

/* Only the part modelled above is pinned; the roster may carry further fields. */
typedef char Overlay7PartySizeCheck[sizeof(Overlay7Party) == 0x14c ? 1 : -1];

/* One row of the item table: the identifiers, the packed price word and the
   five stat deltas the gear rows contribute. */
typedef struct Overlay7Item {
    u16 id;
    u16 unknown_02;
    u16 unknown_04;
    u16 unknown_06;
    u8 unknown_08[4];
    union {
        u32 raw;
        struct { u32 amount : 16, percent : 8, group : 1, reserved : 7; } bits;
    } packed;
    s16 hp, power, defense, speed, stache;
    u16 unknown_1a;
} Overlay7Item;

typedef char Overlay7ItemSizeCheck[sizeof(Overlay7Item) == 0x1c ? 1 : -1];

extern const Overlay7Item data_020505c4[];
extern u8 data_ov007_020905f0[];

#endif
