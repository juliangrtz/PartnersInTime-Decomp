#include "pause_scene_internal.h"
#include <game/overlay007_party.h>

extern "C" {
u16 func_ov007_02075180(Overlay7Party *, int, int);
u8 func_ov007_02075324(Overlay7Party *, int);
int func_ov007_020747e0(Overlay7Party *, int, int, int, int, int, int);
extern const u8 data_ov007_0208dae4[];
void func_ov007_020758f0(Overlay7Party *, u32, int, int, int, int, int, int);
void MIi_CpuCopy16(const void *, void *, u32);

int PauseList_CheckRowAvailability(Overlay7Party *party, int row, int member, int allow_other)
{
    int quantity = party->count;
    if (!quantity || !data_ov007_0208dae4[party->kind])
        return 0;
    quantity = func_ov007_02075180(party, row, 0);
    if (!quantity)
        return -1;
    int item = func_ov007_02075324(party, row);
    return func_ov007_020747e0(party, party->kind, party->unknown_02[1], item,
                             quantity, member, allow_other);
}

u32 PauseList_CopySelectedRow(Overlay7Party *party)
{
    u32 tile = 34 * ((party->tile_row + party->span.offset) % 9) +
               (WORK.main_allocation.offset >> 5);
    u8 *source = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + 32 * tile;
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_SUB) + 640;
    MIi_CpuCopy16(source, destination, 1088);
    return tile >> 1;
}

void PauseList_RedrawSelectedRow(Overlay7Party *party, int copy)
{
    u32 offset = WORK.main_allocation.offset +
                 1088 * ((party->tile_row + party->span.offset) % 9);
    int item = party->slots[(party->first + party->span.offset) % party->count];
    func_ov007_020758f0(party, offset, 0, item, 4, 1, 0, 1);
    if (copy) {
        u8 *source = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + offset;
        u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_SUB) + 640;
        MIi_CpuCopy16(source, destination, 1088);
    }
}

u16 PauseList_DrawSelectedLabel(Overlay7Party *party)
{
    u32 offset = WORK.main_allocation.offset + 9792;
    int item = party->slots[(party->first + party->span.offset) % party->count];
    PauseItem_DrawText(party, offset, 0, item, 4, 1, 0);
    u8 *source = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_MAIN) + offset;
    u8 *destination = (u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_SUB) + 640;
    MIi_CpuCopy16(source, destination, 1088);
    return PauseList_MeasureRowWidth(party, party->span.index, 0);
}

int PauseList_GetTileRow(Overlay7Party *party)
{
    return party->tile_row;
}
}

#include <game/pause_list_row.h>

extern "C" {
u8 func_ov007_020750bc(Overlay7Party *);
void func_ov005_020663d8(int);
void func_ov007_02073c94(int);
void func_ov007_02073994(int);
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
        PauseList_CreateEmptyRowSprite();
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

#include <game/overlay005_resource.h>
#include <game/scene_menu_motion.h>

extern "C" {
void *Overlay5ResourceB_Get(Overlay5ResourceOwner *owner);
void func_ov005_02069084(void *sprite, u8 list);
u8 func_ov007_02075400(Overlay7Party *party);
}

extern "C" void PauseListRow_Refresh(PauseListRowTask *task)
{
    int row = PauseList_GetTileRow((Overlay7Party *)data_ov007_0208e1e4);
    row = (row + task->row) % 9;
    task->x = task->origin_x + (data_ov007_020906f0.x << 12);
    task->y = task->origin_y + (data_ov007_020906f0.y << 12);
    task->tile = (34 * row + (WORK.main_allocation.offset >> 5)) >> 1;
    task->item = func_ov007_02075324((Overlay7Party *)data_ov007_0208e1e4, (u8)task->row);
    task->quantity = func_ov007_02075180((Overlay7Party *)data_ov007_0208e1e4, (u8)task->row, 0);
    if (PauseList_CheckRowAvailability((Overlay7Party *)data_ov007_0208e1e4, (u8)task->row, 4, 0) >= 0)
        task->palette_offset = 0;
    else
        task->palette_offset = 1;
}

extern "C" void PauseListRow_UpdateTextSprite(PauseListSpriteTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    PauseListRowTask *parent = task->parent;
    sprite->attributes_1_bits.tile = parent->tile + (8 * task->selection.segment) / 2;
    sprite->attributes_1_bits.palette =
        (data_ov005_0206a180.palettes[0].bank + parent->palette_offset) & 15;
    sprite->x = parent->x;
    sprite->y = parent->y;
    func_ov005_02069084(sprite, parent->draw_list);
}

extern "C" void PauseListRow_UpdateMarkerSprite(PauseListSpriteTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    PauseListRowTask *parent = task->parent;
    if ((func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 2 &&
         func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 3) || parent->item) {
        sprite->attributes_1_bits.tile = parent->tile + 16;
        sprite->x = parent->x;
        sprite->y = parent->y;
        func_ov005_02069084(sprite, parent->draw_list);
    }
}

extern "C" void PauseListRow_UpdateDigitSprite(PauseListSpriteTask *task)
{
    Overlay5ObjectSprite *sprite =
        (Overlay5ObjectSprite *)Overlay5ResourceB_Get((Overlay5ResourceOwner *)task);
    PauseListRowTask *parent = task->parent;
    if ((func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 2 &&
         func_ov007_02075400((Overlay7Party *)data_ov007_0208e1e4) != 3) || parent->item) {
        int divisor = task->selection.divisor;
        u8 digit = (parent->quantity % (10 * divisor)) / divisor;
        if (digit || divisor == 1) {
            sprite->attributes_1_bits.tile = digit;
            sprite->attributes_1_bits.palette =
                (data_ov005_0206a180.palettes[0].bank + parent->palette_offset) & 15;
            sprite->x = parent->x;
            sprite->y = parent->y;
            func_ov005_02069084(sprite, parent->draw_list);
        }
    }
}
