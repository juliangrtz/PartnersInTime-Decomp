#include "pause_scene_internal.h"
#include <game/overlay007_party.h>

extern "C" {
#include <game/item.h>
extern const BadgeItemRecord data_02050290[];
extern const u8 data_ov007_0208dae4[];
s8 *func_ov007_02075d60(Overlay7Party *, int, u8 *);
u16 func_ov007_020751b4(Overlay7Party *, int, int, int);
int func_ov007_020747e0(Overlay7Party *, int, u32, int, u16, u8, u8);

int PauseItem_RebuildRotatingOrder(Overlay7Party *party, int kind)
{
    u8 limit;
    if (kind == party->kind)
        party->saved_first[party->kind] = party->first;
    func_ov007_02075d60(party, kind, &limit);
    int equipment = 0;
    if ((u8)(kind + 0xfe) <= 1)
        equipment = 1;
    u8 count = 0;
    for (int item = equipment; item < limit; ++item) {
        if (func_ov007_020751b4(party, kind, (u8)item, 0)) {
            party->unknown_88[count++] = item;
        }
    }
    if (equipment && func_ov007_020751b4(party, kind, 0, 0)) {
        party->unknown_88[count++] = 0;
    }
    return count;
}

int PauseItem_CheckRotatedAvailability(Overlay7Party *party, int kind, u32 member, int row, u16 count)
{
    u16 index = (row + party->saved_first[kind]) % count;
    if (!data_ov007_0208dae4[kind])
        return 0;
    int item = party->unknown_88[index];
    u16 quantity = func_ov007_02075d60(party, kind, 0)[item];
    if (!quantity)
        return -1;
    return func_ov007_020747e0(party, kind, member, item, quantity, 4, 0);
}

u16 PauseItem_GetNameId(Overlay7Party *party, int kind, u16 item)
{
    int result = 0;
    switch (kind) {
    case 0: result = gItemRecords[(u16)(item & ITEM_ID_INDEX_MASK)].name_id; break;
    case 1: result = gItemRecords[(u16)(item & ITEM_ID_INDEX_MASK)].name_id; break;
    case 2: result = data_020505c4[(u16)(item & ITEM_ID_INDEX_MASK)].name_id; break;
    case 3: result = data_02050290[(u16)(item & ITEM_ID_INDEX_MASK)].name_id; break;
    case 4: result = gActionItemRecords[(u16)(item & ITEM_ID_INDEX_MASK)].name_id; break;
    }
    return result;
}

u16 PauseItem_GetDescriptionId(Overlay7Party *party, int kind, u16 item)
{
    int result = 0;
    switch (kind) {
    case 0: result = gItemRecords[(u16)(item & ITEM_ID_INDEX_MASK)].description_id; break;
    case 1: result = (u16)(item & ITEM_ID_INDEX_MASK); break;
    case 2: result = data_020505c4[(u16)(item & ITEM_ID_INDEX_MASK)].description_id; break;
    case 3: result = data_02050290[(u16)(item & ITEM_ID_INDEX_MASK)].description_id; break;
    case 4: result = gActionItemRecords[(u16)(item & ITEM_ID_INDEX_MASK)].description_id; break;
    }
    return result;
}
}
