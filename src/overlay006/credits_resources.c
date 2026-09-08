#include "frontend_scene_internal.h"

void CreditsScene_LoadResources(CreditsSceneTask *task)
{
    u32 length;
    CreditsLanguageEntries texture_entries = data_ov006_0207bd00;
    CreditsLanguageEntries palette_entries = data_ov006_0207bd18;
    void *resource;
    int variant;
    MI_CpuFill8(data_ov006_0207c594, 0, sizeof(data_ov006_0207c594));
    func_ov005_02067468(ARCHIVE, 1, data_ov006_0207b5e4[1], 1);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 193, &length, 0);
    Overlay5Display_LoadObjPalette(0, resource, 0, length);
    Overlay5Display_LoadObjPalette(1, resource, 0, length);
    if (resource)
        GameHeap_DeleteArray(resource);
    variant = data_ov006_0207bfe8;
    resource = func_ov005_02066f78(ARCHIVE, 1, data_ov006_0207bed8[variant].entry, &length, 0);
    DC_FlushRange(resource, length);
    func_02038984();
    GX_LoadTex(resource, 0, length);
    func_020387b0();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, data_ov006_0207beda[variant].entry, &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 0, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = func_ov005_02066f78(ARCHIVE, 1, 177, &length, 0);
    DC_FlushRange(resource, length);
    func_02038984();
    GX_LoadTex(resource, 0x10000, length);
    func_020387b0();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 178, &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 1024, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = func_ov005_02066f78(ARCHIVE, 1, (u16)texture_entries.entries[gSaveData[1301]], &length, 0);
    DC_FlushRange(resource, length);
    func_02038984();
    GX_LoadTex(resource, 69632, length);
    func_020387b0();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, (u16)palette_entries.entries[gSaveData[1301]], &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 1152, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = func_ov005_02066f78(ARCHIVE, 1, 191, &length, 0);
    DC_FlushRange(resource, length);
    func_02038984();
    GX_LoadTex(resource, 70144, length);
    func_020387b0();
    if (resource)
        GameHeap_DeleteArray(resource);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 192, &length, 0);
    DC_FlushRange(resource, length);
    func_02038768();
    func_020386f4(resource, 1184, length);
    func_0203869c();
    if (resource)
        GameHeap_DeleteArray(resource);
    func_ov005_02067468(ARCHIVE, 0, data_ov006_0207b5e4[2], 1);
    task->font = Overlay5Archive_ReadEntry(ARCHIVE, 0, 5, 0, 0);
    func_ov005_02067424(ARCHIVE, 0);
    GameAudio_ResetScene();
    GameAudio_LoadMusic(40, 0, 303104);
    GameAudio_ProcessLoading();
    while (GameAudio_IsLoading())
        GameAudio_ProcessLoading();
    GameText_Init(&task->text, &task->font, task->pixels, 0, 0, 0, 1, 0, 1, 4, 31, 1, 255, 0);
}

void CreditsScene_InitGeometry(CreditsSceneTask *unused)
{
    func_02037190();
    func_02037024();
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 0x10;
    REG16(0x04000060) &= 0xCFFB;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    func_02036ee0(0, 0, 0x7FFF, 63, 0);
    REG32(0x04000540) = 0;
    REG32(0x04000580) = 0xBFFF0000;
    func_02037108();
    REG32(0x04000440) = 0;
    REG32(0x04000454) = 0;
    G3i_OrthoW(0, 192 * 4096, 0, 256 * 4096, -1024 * 4096, 1024 * 4096, 4096, 1, 0);
    REG32(0x0400044C) = 0;
}
