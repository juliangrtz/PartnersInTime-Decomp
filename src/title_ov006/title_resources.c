#include "frontend_scene_internal.h"

void TitleScene_SelectGreeting(void)
{
    int slot = data_0205e32c.selected;
    int present = data_0205e32c.present;
    if (!(present & (1 << slot))) {
        slot = 1 - slot;
        if (!(present & (1 << slot))) {
            data_ov006_0207c4ec = 45;
            return;
        }
    }
    {
        const u32 *flags = (const u32 *)SaveStorage_GetSlot(slot);
        data_ov006_0207c4ec = 47;
        if (!((*flags >> 27) & 1)) {
            if ((*flags >> 5) & 1)
                data_ov006_0207c4ec = 45;
            if ((*flags >> 6) & 1)
                data_ov006_0207c4ec = 45;
            if ((*flags >> 7) & 1)
                data_ov006_0207c4ec = 46;
        }
    }
}

void TitleScene_LoadResources(TitleSceneTask *unused)
{
    TitleResourceTable entries = data_ov006_0207b4a4;
    u16 screen[1024];
    int variant, i;
    void *resource;
    TitleScene_SelectGreeting();
    func_ov005_02067468(ARCHIVE, 0, data_ov006_0207b490[1], 1);
    variant = (gSaveData[1301] != 0);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, entries.sets[variant].characters, 0, 0);
    Overlay5DisplayBg_LoadCharactersLZ(1, 2, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, entries.sets[variant].screen, 0, 0);
    MI_UncompressLZ16(resource, screen);
    if (resource)
        GameHeap_DeleteArray(resource);
    for (i = 0; i < 768; ++i)
        screen[i] += 4096;
    Overlay5DisplayBg_LoadScreen(1, 2, screen, 0, 1536);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, entries.sets[variant].palette, 0, 0);
    Overlay5Display_LoadBgPalette(1, resource, 32, 32);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, 142, 0, 0);
    Overlay5DisplayBg_LoadCharactersLZ(0, 3, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, 143, 0, 0);
    Overlay5DisplayBg_LoadScreenLZ(0, 3, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, 144, 0, 0);
    Overlay5Display_LoadBgPalette(0, resource, 0, 32);
    if (resource)
        GameHeap_DeleteArray(resource);
    func_ov005_02067424(ARCHIVE, 0);
    func_ov005_02067468(ARCHIVE, 0, data_ov006_0207b490[0], 1);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 0, 0, 0, 1);
    func_ov005_020698c8(0, func_ov005_02066a7c(ARCHIVE, 0, 0, resource, 0, 0));
    if (resource)
        GameHeap_DeleteArray(resource);
    func_ov005_020671e4(ARCHIVE);
    func_ov005_02067424(ARCHIVE, 0);
}
