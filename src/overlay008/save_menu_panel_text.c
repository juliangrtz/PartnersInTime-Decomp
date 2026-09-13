#include "save_menu_internal.h"

extern u16 data_ov008_0207aa66[2];

typedef struct MenuLocationEntry {
    u16 value, unused;
} MenuLocationEntry;
typedef char MenuLocationEntrySizeCheck[sizeof(MenuLocationEntry) == 4 ? 1 : -1];
extern const MenuLocationEntry data_ov008_02078150[];
extern u16 data_ov008_0207aa6c[];

void SaveMenu_LoadLocationName(int slot, int location)
{
    int entry = data_ov008_02078150[location].value;
    int offset = (slot << 12) / 2 + 49152;
    SaveMenuText_DrawTextureRows(data_ov008_0207828c, &offset, 1, entry, 128, slot);
    data_ov008_0207aa6c[slot] = entry;
}

void SaveMenuText_PreparePanel(int panel, u16 entry)
{
    int offset;
    volatile u32 clear = 0;
    func_0203b7a0(clear, data_ov008_0207828c->pixels, 24576);
    offset = 10752 * panel;
    data_ov008_0207aa66[panel] = SaveMenuText_StreamObjectText(
        data_ov008_0207828c, DISPLAY_ENGINE_SUB, &offset, 2, entry);
}
