#include <nitro.h>

/* The scene's party roster: the rotating display order and the per-slot values
   the interface shows. */
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
} Overlay7Party;

typedef char Overlay7PartySizeCheck[sizeof(Overlay7Party) == 0x10c ? 1 : -1];

/* Per-kind limits, stored as two interleaved columns. */

int func_ov007_02075460(Overlay7Party *party, int last) {
    u8 count = party->count;
    if (count <= 9) return 1;
    if (last == 0) {
        if (party->first == 0) return 1;
    } else if (count - 1 == (party->first + 8) % count) {
        return 1;
    }
    return 0;
}

int func_ov007_02075410(Overlay7Party *party, int last) {
    if (last == 0) {
        if (party->first + party->span.offset == 0) return 1;
    } else if (party->count - 1 == party->first + party->span.offset) {
        return 1;
    }
    return 0;
}

u8 func_ov007_02075408(Overlay7Party *party) {
    return party->span.index;
}

u8 func_ov007_02075400(Overlay7Party *party) {
    return party->kind;
}

