#include "pause_scene_internal.h"
#include <game/overlay007_party.h>

extern "C" {
u16 func_ov007_02075180(Overlay7Party *, int, int);
u8 func_ov007_02075324(Overlay7Party *, int);
int func_ov007_020747e0(Overlay7Party *, int, int, int, int, int, int);
extern const u8 data_ov007_0208dae4[];
void func_ov007_02075b04(Overlay7Party *, u32, int, int, int, int, int);
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
    func_ov007_02075b04(party, offset, 0, item, 4, 1, 0);
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
