#include <game/overlay007_party.h>

u8 func_ov007_02075324(Overlay7Party *, int);
void func_ov007_0207567c(Overlay7Party *, int);
void func_ov005_02069bcc(int, int, int, int);

void PauseList_MoveSelection(Overlay7Party *party, int direction, int wrap)
{
    int item;
    int previous_item = func_ov007_02075324(party, party->span.index);
    int previous_index = (party->first + party->span.offset) % party->count;
    party->span.offset += direction;
    if (party->count <= 9) {
        if (party->span.offset < 0) {
            if (wrap)
                party->span.offset = party->count - 1;
            else
                party->span.offset = 0;
        }
        if (party->span.offset >= party->count) {
            if (wrap)
                party->span.offset = 0;
            else
                party->span.offset = party->count - 1;
        }
    } else {
        if (party->span.offset < 0) {
            party->span.offset = 0;
            if (party->first > 0 || wrap)
                func_ov007_0207567c(party, -1);
        }
        if (party->span.offset >= 9) {
            int count;
            party->span.offset = 8;
            count = party->count;
            if ((party->first + party->span.offset) % count < count - 1 || wrap)
                func_ov007_0207567c(party, 1);
        }
    }
    if (previous_index != (party->first + party->span.offset) % party->count)
        func_ov005_02069bcc(231, 0, 4, 128);
    item = func_ov007_02075324(party, party->span.index);
    if (item == previous_item)
        return;
    PauseItem_DrawText(party, 640, 1, item, 7, 2, 1);
}


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
