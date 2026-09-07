#include <game/inventory.h>

int GameInventory_Add(u16 item, int amount) {
    if (item == 0xffff) {
        int current = *(s32 *)(gSaveData + 1160);
        if (current + amount > 999999) amount = 999999 - current;
        else if (current + amount < 0) amount = -current;
        *(s32 *)(gSaveData + 1160) = current + amount;
    } else if (item == 0xfffe) {
        int current = *(u16 *)(gSaveData + 1164);
        if (current + amount > 999) amount = 999 - current;
        else if (current + amount < 0) amount = -current;
        *(u16 *)(gSaveData + 1164) = current + amount;
    } else {
        s8 *items = 0;
        int maximum = 0;
        int index = item & 0xfff;
        int current;
        switch (item & 0xf000) {
        case 0x1000:
            if (index < 11) { items = (s8 *)gSaveData + 1182; maximum = 99; }
            break;
        case 0x2000:
            if (index < 14) { items = (s8 *)gSaveData + 1166; maximum = 99; }
            break;
        case 0x3000:
            if (index < 41) { items = (s8 *)gSaveData + 1248; maximum = 9; }
            break;
        case 0x4000:
            if (index < 33) { items = (s8 *)gSaveData + 1198; maximum = 9; }
            break;
        }
        if (!items) OS_Terminate();
        current = items[index];
        if (maximum < current + amount) amount = maximum - current;
        else if (current + amount < 0) amount = -current;
        items[index] = current + amount;
    }
    return amount;
}

int GameInventory_GetCount(u16 item) {
    int index;
    s8 *items;
    if (item == 0xffff) return *(s32 *)(gSaveData + 1160);
    if (item == 0xfffe) return *(u16 *)(gSaveData + 1164);
    index = item & 0xfff;
    items = 0;
    switch (item & 0xf000) {
    case 0x1000: if (index < 11) items = (s8 *)gSaveData + 1182; break;
    case 0x2000: if (index < 14) items = (s8 *)gSaveData + 1166; break;
    case 0x3000: if (index < 41) items = (s8 *)gSaveData + 1248; break;
    case 0x4000: if (index < 33) items = (s8 *)gSaveData + 1198; break;
    }
    if (!items) OS_Terminate();
    return items[index];
}
