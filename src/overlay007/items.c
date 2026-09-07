#include <game/overlay007_party.h>
#include <game/save_data.h>
#include <game/text.h>

extern const u8 *func_ov007_02075bbc(Overlay7Party *party, int kind, int mode, int value,
                                     int flag);

u16 func_ov007_02074d14(Overlay7Party *party, u8 kind, int base, int span);
u16 func_ov007_02074c60(Overlay7Party *party, u8 kind, int base, int span);

/* The entry the rotating order currently shows for this kind. */
u16 func_ov007_02074d14(Overlay7Party *party, u8 kind, int base, int span)
{
    u16 index = (base + party->offsets[kind]) % span;
    return party->unknown_88[index];
}

u16 func_ov007_02074c60(Overlay7Party *party, u8 kind, int base, int span)
{
    u16 index = (base + party->offsets[kind]) % span;
    int entry = party->unknown_88[index];
    int mode = 0;
    if (entry == 0 && (u8)(kind + 0xfe) <= 1) {
        mode = 2;
        entry = kind == 2 ? 11 : 13;
    }
    party->text.text = func_ov007_02075bbc(party, kind, mode, entry, 1);
    party->text.cursor.bits.x = 0;
    party->text.cursor.bits.y = 0;
    return GameText_MeasureLine(&party->text);
}

u16 func_ov007_02074c04(Overlay7Party *party, int kind, int value, int flag)
{
    party->text.text = func_ov007_02075bbc(party, kind, 0, value, flag);
    party->text.cursor.bits.x = 0;
    party->text.cursor.bits.y = 0;
    return GameText_MeasureLine(&party->text);
}

u16 func_ov007_02074b9c(Overlay7Party *party, int mode, int value)
{
    party->text.text = func_ov007_02075bbc(party, party->kind, mode, value, 1);
    party->text.cursor.bits.x = 0;
    party->text.cursor.bits.y = 0;
    return GameText_MeasureLine(&party->text);
}

int func_ov007_02074b88(Overlay7Party *party)
{
    return party->count > 9;
}

int func_ov007_02074b44(Overlay7Party *party, int kind, int index, u32 owner)
{
    if (kind == 3) return 1;
    if (index == 0) return 1;
    return (owner >> 1) == data_020505c4[index].packed.bits.group;
}

int func_ov007_02074a80(Overlay7Party *party, int kind, int slot)
{
    u8 count = 0;
    if (kind == 2) {
        int index = 0;
        do {
            if ((data_ov007_020905f0 + index)[0x11a]) {
                if (slot == (gSaveData + (u8)index * 0x24)[0x417]) count = count + 1;
            }
            ++index;
        } while (index < 4);
    } else {
        int index = 0;
        do {
            if ((data_ov007_020905f0 + index)[0x11a]) {
                if (slot == (gSaveData + (u8)index * 0x24)[0x418]) count = count + 1;
            }
            ++index;
        } while (index < 4);
    }
    return count;
}

void func_ov007_02074a00(Overlay7Party *party, int index, s16 *hp, s16 *power, s16 *defense,
                         s16 *speed, s16 *stache)
{
    if (party->kind != 2) return;
    *hp = data_020505c4[index].hp;
    *power = data_020505c4[index].power;
    *defense = data_020505c4[index].defense;
    *speed = data_020505c4[index].speed;
    *stache = data_020505c4[index].stache;
}
