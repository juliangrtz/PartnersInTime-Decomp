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
        int column;
        int row;
        for (row = 0; row < 9; ++row) {
            for (column = 0; column < 26; ++column) {
                screen[column + 32 * row] += tile;
                ++tile;
            }
        }
    }
    Overlay5DisplayBg_SetOffset(0, 2, -24, -68);
}
void EraseScene_LoadResources(SaveSceneTask *unused)
{
    MenuAssetDescriptor asset = data_ov008_020781e0;
    u32 length;
    void *resource;
    u16 *screen;
    int i;
    SaveMenuText *text;
    MI_CpuFill8(&data_ov008_02078290, 0, sizeof(data_ov008_02078290));
    GameSpriteAllocation_Allocate(&data_ov005_0206a1b0, 0, 0, 192, 0, 0xFFFF, 1, 0);
    GameSpriteAllocation_Allocate(&data_ov005_0206a1c8, 1, 0, 1008, 0, 0xFFFF, 1, 0);
    func_02007ebc(&data_ov005_0206a1e0, 0, 0, 16, 0, 2, 0, 0, 0, 0xFFFF);
    func_02007ebc(&data_ov005_0206a1f4, 1, 0, 16, 0, 2, 0, 0, 0, 0xFFFF);
    func_ov005_02067468(ARCHIVE, 1, data_ov008_020781d8[1], 1);
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
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 135, &length, 0);
    Overlay5Display_LoadObjPalette(0, resource, 0, length);
    Overlay5Display_LoadBgPalette(0, resource, 32, length);
    if (resource)
        GameHeap_DeleteArray(resource);
    for (i = 0; i < 2; ++i)
        *(u16 *)Overlay5Display_GetBgPalette((u8)i) = 0;
    func_ov005_02067424(ARCHIVE, 1);
    func_ov005_02067468(ARCHIVE, 2, data_ov008_020781d8[0], 1);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 2, 0, 0, 1);
    func_ov005_020698c8(0, func_ov005_02066a7c(ARCHIVE, 2, (u16)asset.entry, resource, (u8)asset.mode, 0));
    if (resource)
        GameHeap_DeleteArray(resource);
    func_ov005_020671e4(ARCHIVE);
    text = GameHeap_New(sizeof(SaveMenuText), 0, 0, 0);
    if (text)
        text = SaveMenuText_Init(text);
    data_ov008_0207828c = text;
}
