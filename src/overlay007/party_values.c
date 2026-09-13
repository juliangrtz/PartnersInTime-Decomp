#include <game/overlay007_party.h>
#include <game/save_data.h>
#include <game/item.h>
extern const BadgeItemRecord data_02050290[];

extern u8 data_ov007_020905f0[];

/* Per-kind limits, stored as two interleaved columns. */
extern const u8 data_ov007_0208dad4[];
extern const u8 data_ov007_0208daf4[], data_ov007_0208daf5[];


int func_ov007_02075244(Overlay7Party *party, int kind, int slot);
int func_ov007_020752b4(Overlay7Party *party, int kind, int slot);
u16 func_ov007_020751b4(Overlay7Party *party, u8 kind, int slot, int subtract);

int PauseItem_GetMarkerId(Overlay7Party *party, int kind, int item, u32 member)
{
    int result = 0;
    switch (kind) {
    case 0: result = gItemRecords[item].unknown_08[1]; break;
    case 1: result = -1; break;
    case 2:
        result = data_020505c4[item].unknown_08[1];
        if (item == 0)
            result = (member >> 1) ? 12 : 11;
        break;
    case 3: result = data_02050290[item].unknown_06[3]; break;
    case 4: result = gActionItemRecords[item].unknown_08[1]; break;
    }
    return result;
}

u8 func_ov007_02075324(Overlay7Party *party, int offset) {
    return party->slots[(party->first + offset) % party->count] & 0xfff;
}

int func_ov007_020752b4(Overlay7Party *party, int kind, int slot) {
    u8 count = 0;
    int index = 0;
    do {
        if ((data_ov007_020905f0 + index)[0x11a]) {
            u8 member;
            if (kind == 2) member = (gSaveData + (u8)index * 0x24)[0x417];
            else member = (gSaveData + (u8)index * 0x24)[0x418];
            if (member == slot) count = count + 1;
        }
        ++index;
    } while (index < 4);
    return count;
}

int func_ov007_02075244(Overlay7Party *party, int kind, int slot) {
    u8 count = 0;
    int index = 0;
    do {
        if (!(data_ov007_020905f0 + index)[0x11a]) {
            u8 member;
            if (kind == 2) member = (gSaveData + (u8)index * 0x24)[0x417];
            else member = (gSaveData + (u8)index * 0x24)[0x418];
            if (member == slot) count = count + 1;
        }
        ++index;
    } while (index < 4);
    return count;
}

u16 func_ov007_020751b4(Overlay7Party *party, u8 kind, int slot, int subtract) {
    s8 value = PauseItem_GetValues(party, kind, 0)[slot];
    if ((u8)(kind + 0xfe) <= 1) {
        value = value - func_ov007_02075244(party, kind, slot);
        if (subtract) value = value - func_ov007_020752b4(party, kind, slot);
        if (value < 0) value = 0;
        if (slot == 0) value = 99;
    }
    return (u16)value;
}

u16 func_ov007_02075180(Overlay7Party *party, int value, int subtract) {
    return func_ov007_020751b4(party, party->kind, func_ov007_02075324(party, value), subtract);
}

u16 func_ov007_020750ec(Overlay7Party *party, int offset, int amount) {
    u8 slot = party->slots[(party->first + offset) % party->count];
    party->values[slot] += amount;
    if (party->values[slot] < 0) party->values[slot] = 0;
    if (party->values[slot] > data_ov007_0208dad4[party->kind]) {
        party->values[slot] = data_ov007_0208dad4[party->kind];
    }
    return (u16)party->values[slot];
}

u8 func_ov007_020750d8(Overlay7Party *party) {
    return data_ov007_0208daf4[party->kind * 2];
}

u8 func_ov007_020750c4(Overlay7Party *party) {
    return data_ov007_0208daf5[party->kind * 2];
}

u8 func_ov007_020750bc(Overlay7Party *party) {
    return party->count;
}
