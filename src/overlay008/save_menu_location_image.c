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
void SaveMenu_BuildInversePalette(int slot)
{
    u16 *palette = data_ov008_02078a64[slot];
    int i;
    GameResource_Move16(data_ov008_02078464[slot], palette, 512);
    for (i = 0; i < 256; ++i) {
        *palette = (u8)(31 - (*palette & 31)) +
                   (((u8)(31 - ((*palette >> 10) & 31)) << 10) + ((u8)(31 - ((*palette >> 5) & 31)) << 5));
        ++palette;
    }
}

void SaveMenu_LoadLocationImage(int slot, int location)
{
    u32 entry = data_ov008_02078152[location].value;
    int first;
    u32 length;
    void *resource;
    if (entry >= 33)
        entry = 32;
    first = 3 * entry;
    data_ov008_0207844c[slot] =
        func_ov005_02066f78(MENU_ARCHIVE, 3, (u16)first, &data_ov008_02078458[slot], 0);
    resource = Overlay5Archive_ReadEntry(MENU_ARCHIVE, 3, (u16)(first + 2), &length, 0);
    GameResource_Move16(resource, data_ov008_02078464[slot], length);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = func_ov005_02066f78(MENU_ARCHIVE, 3, (u16)(first + 1), &length, 0);
    GameResource_Move16(resource, data_ov008_02079264[slot], length);
    if (resource)
        GameHeap_DeleteArray(resource);
    SaveMenu_BuildInversePalette(slot);
}
