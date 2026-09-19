#include "nawatobi_internal.h"

extern "C" {
void NawatobiScene_LoadResources(NawatobiSceneTask *)
{
    FourResources resources = data_ov007_0208e12c;
    ThreeResources scene_resources = data_ov007_0208e0f4;
    ClearFast((void *)0x06000000, 16384);
    ClearFast((void *)0x06200000, 16384);
    data_ov007_020a6bc4 = (u8 *)GameHeap_NewArray(400, 0, 0, 0);
    data_ov007_020a6bc0 = (u8 *)GameHeap_NewArray(400, 0, 0, 0);
    data_ov007_020a6bc8 = (u8 *)GameHeap_NewArray(102400, 0, 0, 0);
    Clear16(data_ov007_020a6bc8, 102400);
    GameSpriteAllocation_Allocate(&DISPLAY.allocations[0], 0, 0, 32, 0, 65535, 1, 0);
    GameSpriteAllocation_Allocate(&DISPLAY.allocations[1], 1, 0, 32, 0, 65535, 1, 0);
    func_02007ebc(&DISPLAY.palettes[0], 0, 0, 16, 0, 1, 0, 0, 0, 65535);
    func_02007ebc(&DISPLAY.palettes[1], 1, 0, 16, 0, 1, 0, 0, 0, 65535);
    func_ov005_02067468(ARCHIVE, 1, data_ov007_0208e074[2], 1);
    void *buffer = Overlay5Archive_ReadEntry(ARCHIVE, 1, 22, 0, 0);
    Overlay5Display_LoadObjTilesLZ(DISPLAY_ENGINE_MAIN, buffer, 0);
    Overlay5Display_LoadObjTilesLZ(DISPLAY_ENGINE_SUB, buffer, 0);
    DeleteBuffer(buffer);
    buffer = Overlay5Archive_ReadEntry(ARCHIVE, 1, 23, 0, 0);
    Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_MAIN, buffer, 0, 32);
    Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_SUB, buffer, 0, 32);
    Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_MAIN, buffer, 0, 32);
    Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, buffer, 0, 32);
    DeleteBuffer(buffer);
    func_ov005_02067468(ARCHIVE, 2, data_ov007_0208e074[0], 1);
    {
        void *table = Overlay5Archive_ReadEntry(ARCHIVE, 2, 0, 0, 1);
        for (int i = 0; i < 4; ++i) {
            Overlay5Asset *asset = func_ov005_02066a7c(ARCHIVE, 2, (u16)resources.entries[i][0], table,
                                                       (u8)resources.entries[i][1], 0);
            func_ov005_020698c8(i + 72, asset);
        }
        DeleteBuffer(table);
    }
    func_ov005_020671e4(ARCHIVE);
    func_ov005_02067424(ARCHIVE, 2);
    func_ov005_02067468(ARCHIVE, 2, data_ov007_0208e074[1], 1);
    {
        int i;
        void *table = Overlay5Archive_ReadEntry(ARCHIVE, 2, 0, 0, 1);
        i = 0;
        do {
            Overlay5Asset *asset = func_ov005_02066a7c(ARCHIVE, 2, (u16)scene_resources.entries[i][0], table,
                                                       (u8)scene_resources.entries[i][1], 0);
            func_ov005_020698c8(i + 36, asset);
            ++i;
        } while (i < 3);
        DeleteBuffer(table);
    }
    func_ov005_020671e4(ARCHIVE);
}

void NawatobiMenu_Draw(void)
{
    GameConsole_Clear(CONSOLE(1));
    GameConsole_Clear(CONSOLE(0));
    GameConsole_SetCursor(CONSOLE(1), 6, 10);
    GameConsole_Printf(CONSOLE(1), data_ov007_0208e19c);
    int level = 0;
    int row = 8;
    do {
        if ((s8)data_ov007_020a6bcc[NAWATOBI_STATE_SELECTED_LEVEL] == level) {
            GameConsole_SetCursor(CONSOLE(0), 8, row);
            GameConsole_Printf(CONSOLE(0), data_ov007_0208e1b4);
        }
        GameConsole_SetCursor(CONSOLE(0), 12, row);
        GameConsole_Printf(CONSOLE(0), data_ov007_0208e1b8, level + 1);
        if (level >= 2)
            GameConsole_Printf(CONSOLE(0), data_ov007_0208e1c4);
        ++level;
        row += 2;
    } while (level < 5);
}
}
