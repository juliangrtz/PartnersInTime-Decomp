#include <game/overlay007_party.h>

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
