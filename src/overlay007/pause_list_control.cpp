#include "pause_scene_internal.h"
#include <game/overlay007_party.h>

extern "C" {
u8 func_ov007_02075324(Overlay7Party *, int);
u8 func_ov007_020750bc(Overlay7Party *);
void func_ov005_020663d8(int);
void func_ov007_02073c94(int);
void func_ov007_02073994(int);
void func_ov007_02073428(void);
void func_ov007_0207355c(void);
extern const void *data_ov007_0208dad0[];
void func_ov005_02067468(Overlay5Archive *, u8, const void *, int);
void *Overlay5Archive_ReadEntry(Overlay5Archive *, int, u16, u32 *, int);
}

extern "C" void PauseList_Show(Overlay7Party *party)
{
    party->visible = 1;
    GameSpriteAllocation_Allocate(&WORK.main_allocation, 0, 0, 340, 0, 65535, 1, 0);
    PauseList_Prepare(party, WORK.selected_menu);
    for (int row = 0; row < 9; ++row) {
        u32 count = party->count;
        if (count < 9 && party->first + row >= (int)count)
            break;
        func_ov007_02073c94(row);
    }
    int kind = party->kind;
    if ((u8)(kind + 0xfe) <= 1) {
        func_ov007_02073994(kind);
        func_ov007_02073428();
    }
    if (party->kind == 1 && func_ov007_020750bc(party)) {
        func_ov005_02067468(ARCHIVE, 3, data_ov007_0208dad0[0], 1);
        party->list_images = Overlay5Archive_ReadEntry(ARCHIVE, 3, 0, 0, 1);
        func_ov007_0207355c();
    }
}

extern "C" void PauseList_Hide(Overlay7Party *party)
{
    party->visible = 0;
    party->saved_first[party->kind] = party->first;
    party->saved_selected[party->kind] = party->span.offset;
    func_ov005_020663d8(2);
    if (party->kind == 1 && func_ov007_020750bc(party)) {
        if (party->list_images) {
            GameHeap_DeleteArray(party->list_images);
            party->list_images = 0;
        }
    }
    GameSpriteAllocation_Unlink(&WORK.main_allocation);
}

extern "C" u16 PauseList_MeasureRowWidth(Overlay7Party *party, int row, int plural)
{
    int item = func_ov007_02075324(party, row);
    int mode = 0;
    if (!item) {
        int kind = party->kind;
        if ((u8)(kind + 0xfe) <= 1) {
            mode = 2;
            if (kind == 2)
                item = 11;
            else
                item = 13;
        }
    }
    party->text.text = PauseItem_GetText(party, party->kind, mode, item, plural);
    party->text.cursor.bits.x = 0;
    party->text.cursor.bits.y = 0;
    return GameText_MeasureLine(&party->text);
}
