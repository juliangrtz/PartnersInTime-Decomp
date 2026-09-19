/*
 * Save menu resources (overlay 8, 0x02071D30-0x020727D8).
 *
 * Loads the save menu's archives, the location preview images and the text
 * background, and builds the inverted palette the highlighted rows use. Freeing
 * releases the three owned buffers the workspace holds.
 */

#include "save_menu_internal.h"

typedef struct SaveMenuResourceView {
    u8 unknown_00[444];
    void *location_characters[3];
} SaveMenuResourceView;
void SaveMenu_FreeResources(void)
{
    int i;
    SaveMenuText *text = data_ov008_0207828c;
    if (text) {
        SaveMenuText_DeleteOwned(text);
        data_ov008_0207828c = 0;
    }
    for (i = 0; i < 3; ++i) {
        if (((SaveMenuResourceView *)data_ov008_02078290)->location_characters[i]) {
            GameHeap_DeleteArray(((SaveMenuResourceView *)data_ov008_02078290)->location_characters[i]);
            ((SaveMenuResourceView *)data_ov008_02078290)->location_characters[i] = 0;
        }
    }
}

#include "menu_resource_internal.h"

static inline void InitTextBackground(void)
{
    u16 *screen;
    int i;
    {
        void *characters = Overlay5DisplayBg_GetCharacterVram(0, 2);
        volatile u32 clear = 0;
        func_0203b7a0(clear, characters, 7520);
    }
    screen = Overlay5DisplayBg_GetScreenVram2K(0, 2);
    for (i = 0; i < 1024; ++i)
        screen[i] = 4096;
    screen = Overlay5DisplayBg_GetScreenVram2K(0, 2);
    {
        u16 tile = 1;
        int row;
        for (row = 0; row < 9; ++row) {
            int column;
            for (column = 0; column < 26; ++column) {
                screen[column + 32 * row] += tile;
                ++tile;
            }
        }
    }
    Overlay5DisplayBg_SetOffset(0, 2, -24, -68);
}
void SaveMenu_LoadResources(void)
{
    u32 length;
    MenuAssetTable assets = data_ov008_02078044;
    void *resource;
    u16 *screen;
    int i;
    SaveMenuText *text;
    MI_CpuFill8(data_ov008_02078290, 0, sizeof(MenuResourceWork));
    GameSpriteAllocation_Allocate(&data_ov005_0206a1b0, 0, 0, 192, 0, 0xFFFF, 1, 0);
    GameSpriteAllocation_Allocate(&data_ov005_0206a1c8, 1, 0, 1008, 0, 0xFFFF, 1, 0);
    func_02007ebc(&data_ov005_0206a1e0, 0, 0, 16, 0, 2, 0, 0, 0, 0xFFFF);
    func_02007ebc(&data_ov005_0206a1f4, 1, 0, 16, 0, 2, 0, 0, 0, 0xFFFF);
    func_ov005_02067468(ARCHIVE, 1, data_ov008_02077ffc[1], 1);
    func_ov005_02067468(ARCHIVE, 3, data_ov008_02077ffc[2], 1);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 116, 0, 0);
    Overlay5DisplayBg_LoadCharactersLZ(0, 3, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 118, 0, 0);
    DC_FlushRange(resource, 512);
    Overlay5Display_BeginLoadBgExtPalette(0);
    Overlay5Display_LoadBgExtPalette(0, resource, 16384, 512);
    Overlay5Display_LoadBgExtPalette(0, resource, 24576, 512);
    Overlay5Display_EndLoadBgExtPalette(0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 117, 0, 0);
    Overlay5DisplayBg_LoadScreenLZ(0, 3, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 119, 0, 0);
    Overlay5DisplayBg_LoadCharactersLZ(0, 1, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 121, &length, 0);
    ((u8 *)resource)[0] = 0;
    ((u8 *)resource)[1] = 0;
    Overlay5Display_LoadBgPalette(0, resource, 0, length);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 120, 0, 0);
    Overlay5DisplayBg_LoadScreenLZ(0, 1, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    InitTextBackground();
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 122, 0, 0);
    Overlay5DisplayBg_LoadCharactersLZ(1, 1, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 124, 0, 0);
    DC_FlushRange(resource, 512);
    Overlay5Display_BeginLoadBgExtPalette(1);
    Overlay5Display_LoadBgExtPalette(1, resource, 16384, 512);
    Overlay5Display_LoadBgExtPalette(1, resource, 24576, 512);
    Overlay5Display_EndLoadBgExtPalette(1);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 123, 0, 0);
    Overlay5DisplayBg_LoadScreenLZ(1, 1, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = func_ov005_02066f78(ARCHIVE, 1, 125, &length, 0);
    DC_FlushRange(resource, length);
    func_02038984();
    GX_LoadTex(resource, 0, length);
    func_020387b0();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 126, &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 0, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    {
        MenuLanguageEntries localized = data_ov008_0207802c;
        resource = func_ov005_02066f78(ARCHIVE, 1, (u16)localized.entries[gSaveData[1301]], &length, 0);
        DC_FlushRange(resource, length);
        func_02038984();
        GX_LoadTex(resource, 32768, length);
        func_020387b0();
        if (resource)
            GameHeap_DeleteArray(resource);
    }
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 133, &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 64, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = func_ov005_02066f78(ARCHIVE, 1, 134, &length, 0);
    DC_FlushRange(resource, length);
    func_02038984();
    GX_LoadTex(resource, 40960, length);
    func_020387b0();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 64, &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 96, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 135, &length, 0);
    ((u8 *)resource)[0] = 0;
    ((u8 *)resource)[1] = 0;
    Overlay5Display_LoadObjPalette(0, resource, 0, length);
    Overlay5Display_LoadObjPalette(1, resource, 0, length);
    Overlay5Display_LoadBgPalette(0, resource, 32, length);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 160, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    func_ov005_02067468(ARCHIVE, 2, data_ov008_02077ffc[0], 1);
    {
        void *asset_table = Overlay5Archive_ReadEntry(ARCHIVE, 2, 0, 0, 1);
        int asset_index;
        for (asset_index = 0; asset_index < 3; ++asset_index) {
            Overlay5Asset *asset = func_ov005_02066a7c(ARCHIVE, 2, (u16)assets.assets[asset_index].entry,
                                                       asset_table, (u8)assets.assets[asset_index].mode, 0);
            func_ov005_020698c8(asset_index, asset);
        }
        if (asset_table)
            GameHeap_DeleteArray(asset_table);
    }
    func_ov005_020671e4(ARCHIVE);
    text = GameHeap_New(sizeof(SaveMenuText), 0, 0, 0);
    if (text)
        text = SaveMenuText_Init(text);
    data_ov008_0207828c = text;
}

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
