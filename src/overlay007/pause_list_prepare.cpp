#include "pause_scene_internal.h"
#include <game/overlay007_party.h>

extern "C" {
extern u8 gSceneMenuSubmenus[];
u16 func_ov007_020751b4(Overlay7Party *, u8, int, int);
void func_ov007_02075e10(Overlay7Party *);

void PauseList_Prepare(Overlay7Party *party, int menu)
{
    party->member = WORK.selected[0];
    int sub = gSceneMenuSubmenus[menu];
    switch (menu) {
    case 0:
        switch (sub) {
        case 0: party->kind = 0; break;
        case 1: party->kind = 1; break;
        }
        break;
    case 1:
        switch (sub) {
        case 0: party->kind = 2; break;
        case 1: party->kind = 3; break;
        }
        break;
    case 3: party->kind = 4; break;
    default: party->kind = 0; break;
    }
    party->values = PauseItem_GetValues(party, party->kind, &party->item_limit);
    party->tile_row = 0;
    party->first = party->saved_first[party->kind];
    party->span.offset = party->saved_selected[party->kind];
    MI_CpuFill8(party->slots, 0, sizeof(party->slots));
    int first = 0;
    if ((u8)(party->kind + 254) <= 1)
        first = 1;
    party->count = 0;
    for (int item = first; item < party->item_limit; ++item) {
        if (func_ov007_020751b4(party, party->kind, (u8)item, 0))
            party->slots[party->count++] = item;
    }
    if (first && func_ov007_020751b4(party, party->kind, 0, 0))
        party->slots[party->count++] = 0;
    if (party->first + 9 >= party->count) {
        if (party->count >= 9) {
            party->first = party->count - 9;
        } else {
            party->first = 0;
            if (party->span.offset >= party->count)
                party->span.offset = party->count - 1;
        }
    }
    func_ov007_02075e10(party);
}
}
