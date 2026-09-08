#include "save_menu_internal.h"

typedef struct MenuLocationEntry {
    u16 value, unused;
} MenuLocationEntry;
extern const MenuLocationEntry data_ov008_02078150[], data_ov008_02078152[];
extern u16 data_ov008_0207aa6c[];
extern u8 data_ov008_0207aa64[];
extern void *data_ov008_0207844c[];
extern u32 data_ov008_02078458[];
extern u16 data_ov008_02078464[][256], data_ov008_02078a64[][256], data_ov008_02079264[][1024];
extern void *func_ov005_02066f78(Overlay5Archive *, int, u16, u32 *, int);
void SaveMenu_LoadLocationName(int slot, int location)
{
    int entry = data_ov008_02078150[location].value;
    int offset = (slot << 12) / 2 + 49152;
    SaveMenuText_DrawTextureRows(data_ov008_0207828c, &offset, 1, entry, 128, slot);
    data_ov008_0207aa6c[slot] = entry;
}
