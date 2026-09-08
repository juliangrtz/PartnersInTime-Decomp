#include "shop_scene_internal.h"
extern "C" {
#include <game/script_vm.h>
typedef struct ShopAssets {
    u32 entries[6][2];
} ShopAssets;
typedef char ShopAssetsSizeCheck[sizeof(ShopAssets) == 48 ? 1 : -1];
extern ShopAssets data_ov009_0207e140;
extern const void *data_ov009_0207e054[];
extern GameSpriteAllocation data_ov005_0206a1b0, data_ov005_0206a1c8;
extern u8 data_ov005_0206a1e0[], data_ov005_0206a1f4[], data_ov005_0206a208[], data_ov005_0206a21c[];
extern void *func_ov005_0206964c(void);
extern void func_ov009_0206e8ac(ShopSceneTask *), func_ov009_0207bd84(void), func_ov009_0207cc8c(void);
extern void func_0203b7a0(u32, void *, u32), MIi_CpuClearFast(u32, void *, u32),
    MIi_CpuCopy16(const void *, void *, u32);
extern void DC_FlushRange(const void *, u32);
extern void func_02007ebc(void *, int, int, int, int, int, int, int, int, int);
extern void func_ov005_02067468(Overlay5Archive *, u8, const void *, int);
extern void *Overlay5Archive_ReadEntry(Overlay5Archive *, u8, int, u32 *, int),
    *func_ov005_02066f78(Overlay5Archive *, int, u16, u32 *, int);
extern void *func_ov005_0206687c(const void *, u32 *, int);
extern Overlay5Asset *func_ov005_02066a7c(Overlay5Archive *, u16, u16, const void *, u8, int);
extern void func_ov005_020698c8(u32, Overlay5Asset *);
#define WORK data_ov009_0207ea3c
#define SAVED_SHOP (((ShopSavedState *)gSaveData)->shop)
void ShopScene_LoadResources(ShopSceneTask *task)
{
    ShopAssets assets = data_ov009_0207e140;
    u32 length;
    void *resource;
    u16 *screen;
    int i;
    WORK.initialized = 1;
    WORK.selected_category = 0;
    func_ov009_0206e8ac(task);
    if (WORK.special_shop == 1) {
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 0);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 0, 1);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 0, 1);
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 1);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 1, 0);
        Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_SUB, 1, 1);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 1, 1);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 1, 3);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 1, 0);
        Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 1, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 1, 0);
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 2);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 2, 1);
        Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 2, 0);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 2, 2);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 2, 3);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 2, 0);
        Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 2, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 2);
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 3);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 3, 1);
        Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 3, 0);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 3, 3);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 3, 4);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 3, 0);
        Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 3, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 3, 3);
    } else {
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 0);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 0, 1);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 0, 0);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 0, 1);
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 1);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 1, 0);
        Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_SUB, 1, 1);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 1, 1);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 1, 2);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 1, 0);
        Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 1, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 1, 0);
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 2);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 2, 1);
        Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 2, 128);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 2, 5);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 2, 0);
        Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 2, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 2);
        Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 3);
        Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 3, 1);
        Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 3, 0);
        Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 3, 3);
        Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 3, 3);
        Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 3, 0);
        Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 3, 1);
        Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 3, 3);
    }

    for (i = 0; i < 4; ++i)
        WORK.available[i] = 1;
    WORK.party_count = 4;
    if (VM_ReadVariable(8197, 0, 0)) {
        WORK.available[1] = 0;
        WORK.available[2] = 0;
        WORK.available[3] = 0;
        WORK.party_count = 1;
    }
    if (VM_ReadVariable(8198, 0, 0)) {
        WORK.available[2] = 0;
        WORK.available[3] = 0;
        WORK.party_count = 2;
    }
    if (VM_ReadVariable(8199, 0, 0)) {
        WORK.available[0] = 0;
        WORK.available[1] = 0;
        WORK.party_count = 2;
    }
    for (int member = 0; member < 4; ++member)
        WORK.displayed[member] = WORK.available[member];
    if (VM_ReadVariable(8219, 0, 0))
        for (int member = 0; member < 4; ++member)
            WORK.displayed[member] = 1;
    WORK.unknown_8c0 = 0;
    WORK.unknown_8c1 = 2;
    WORK.selected_member = 0;
    for (int member = 0; member < 4; ++member) {
        if (WORK.available[member])
            break;
        WORK.selected_member = member + 1;
    }
    for (i = 0; i < 60; ++i)
        WORK.sprites[i] = func_ov005_0206964c();
    WORK.owned_84[0] = GameHeap_NewArray(5120, 0, 0, 0);
    WORK.owned_84[1] = GameHeap_NewArray(5120, 0, 0, 0);
    WORK.owned_58[5] = GameHeap_NewArray(6144, 0, 0, 0);
    if (!WORK.special_shop && WORK.animated_background) {
        volatile u32 clear;
        WORK.owned_58[7] = GameHeap_NewArray(53248, 0, 0, 0);
        clear = 0;
        func_0203b7a0(clear, WORK.owned_58[7], 49152);
        WORK.background_dirty = 1;
        Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, 4);
    }
    if (WORK.special_shop == 1)
        WORK.owned_58[6] = GameHeap_NewArray(14336, 0, 0, 0);
    WORK.owned_58[3] = GameHeap_NewArray(2816, 0, 0, 0);
    WORK.owned_58[4] = GameHeap_NewArray(5632, 0, 0, 0);
    GameSpriteAllocation_Allocate(&data_ov005_0206a1b0, 0, 0, 808, 0, 0xffff, 1, 0);
    GameSpriteAllocation_Allocate(&data_ov005_0206a1c8, 1, 0, !WORK.animated_background ? 560 : 804, 0,
                                  0xffff, 1, 0);
    func_02007ebc(data_ov005_0206a1e0, 0, 0, 16, 0, 4, 0, 0, 0, 0xffff);
    func_02007ebc(data_ov005_0206a1f4, 1, 0, 16, 0, 4, 0, 0, 0, 0xffff);
    func_02007ebc(data_ov005_0206a208, 0, 0, 0, 0, 16, 0, 0, 0, 0xffff);
    func_02007ebc(data_ov005_0206a21c, 1, 0, 0, 0, 16, 0, 0, 0, 0xffff);
    if (!SAVED_SHOP) {
        {
            volatile u32 clear = 0;
            MIi_CpuClearFast(clear, (void *)0x06000000, 16384);
        }
        {
            volatile u32 clear = 0;
            MIi_CpuClearFast(clear, (void *)0x06200000, 16384);
        }
    }
    func_ov005_02067468(ARCHIVE, 1, data_ov009_0207e054[1], 1);
    ShopScene_LoadBackgrounds(task);
    resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 84, 0, 0);
    Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_MAIN, 1, resource, 0);
    if (WORK.special_shop == 1)
        Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_SUB, 1, resource, 0);
    if (resource)
        GameHeap_DeleteArray(resource);
    if (!SAVED_SHOP) {
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 85, 0, 0);
        DC_FlushRange(resource, 512);
        Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, resource, 16896, 512);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, resource, 25088, 512);
        Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
        Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_SUB);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_SUB, resource, 16896, 512);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_SUB, resource, 25088, 512);
        Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_SUB);
        if (resource)
            GameHeap_DeleteArray(resource);
    }
    if (!WORK.special_shop) {
        if (WORK.animated_background == 2)
            resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 91, 0, 0);
        else
            resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 87, 0, 0);
    } else
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 89, 0, 0);
    Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_MAIN, 1, resource, 0);
    screen = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_MAIN, 1);
    for (i = 0; i < 768; ++i, ++screen)
        *screen += 4096;
    if (resource)
        GameHeap_DeleteArray(resource);
    if (!SAVED_SHOP) {
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 86, 0, 0);
        Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_MAIN, 2, resource, 0);
        screen = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_MAIN, 2);
        for (i = 0; i < 768; ++i, ++screen)
            *screen += 4096;
        if (resource)
            GameHeap_DeleteArray(resource);
    }
    if (WORK.special_shop == 1) {
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 90, 0, 0);
        Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_SUB, 2, resource, 0);
        screen = (u16 *)Overlay5DisplayBg_GetScreenVram2K(DISPLAY_ENGINE_SUB, 2);
        for (i = 0; i < 768; ++i, ++screen)
            *screen += 4096;
        if (resource)
            GameHeap_DeleteArray(resource);
        Overlay5Display_EnablePlanes(DISPLAY_ENGINE_SUB, 4);
    }
    if (!WORK.special_shop && WORK.animated_background)
        func_ov009_0207bd84();
    WORK.owned_58[1] = func_ov005_02066f78(ARCHIVE, 1, 69, 0, 0);
    if (!SAVED_SHOP) {
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 70, &length, 0);
        ((u8 *)resource)[0] = 0;
        ((u8 *)resource)[1] = 0;
        DC_FlushRange(resource, length);
        Overlay5Display_BeginLoadObjExtPalette(DISPLAY_ENGINE_MAIN);
        Overlay5Display_LoadObjExtPalette(DISPLAY_ENGINE_MAIN, resource, 0, length);
        Overlay5Display_EndLoadObjExtPalette(DISPLAY_ENGINE_MAIN);
        Overlay5Display_BeginLoadObjExtPalette(DISPLAY_ENGINE_SUB);
        Overlay5Display_LoadObjExtPalette(DISPLAY_ENGINE_SUB, resource, 0, length);
        Overlay5Display_EndLoadObjExtPalette(DISPLAY_ENGINE_SUB);
        if (resource)
            GameHeap_DeleteArray(resource);
    }
    if (!SAVED_SHOP) {
        if (gSaveData[1301] != 3 && gSaveData[1301] != 5)
            WORK.owned_58[2] = func_ov005_02066f78(ARCHIVE, 1, 104, 0, 0);
        else
            WORK.owned_58[2] = func_ov005_02066f78(ARCHIVE, 1, 105, 0, 0);
        Overlay5Display_LoadObjTiles(DISPLAY_ENGINE_MAIN, (u8 *)WORK.owned_58[2] + 2816, 21760, 512);
        Overlay5Display_LoadObjTiles(DISPLAY_ENGINE_SUB, WORK.owned_58[2], 2432, 3328);
    } else {
        WORK.owned_58[2] = GameHeap_NewArray(3328, 0, 0, 0);
        MIi_CpuCopy16((u8 *)Overlay5Display_GetObjVram(DISPLAY_ENGINE_SUB) + 2432, WORK.owned_58[2], 3328);
    }
    func_ov009_0207cc8c();
    if (!SAVED_SHOP) {
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 65, 0, 0);
        Overlay5Display_LoadObjTilesLZ(DISPLAY_ENGINE_SUB, resource, 24704);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 66, 0, 0);
        Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_SUB, resource, 96, 64);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 71, &length, 0);
        ((u8 *)resource)[0] = 0;
        ((u8 *)resource)[1] = 0;
        Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_MAIN, resource, 0, length);
        Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_SUB, resource, 0, length);
        Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_MAIN, resource, 0, length);
        if (WORK.special_shop == 1)
            Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, resource, 0, length);
        else
            Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, resource, 32, length);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = Overlay5Archive_ReadEntry(ARCHIVE, 1, 106, &length, 0);
        Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_MAIN, resource, 64, length);
        Overlay5Display_LoadObjPalette(DISPLAY_ENGINE_SUB, resource, 64, length);
        Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, resource, 96, length);
        if (resource)
            GameHeap_DeleteArray(resource);
    } else {
        resource = Overlay5Display_GetObjPalette(DISPLAY_ENGINE_MAIN);
        if (WORK.special_shop == 1)
            Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, resource, 0, 64);
        else
            Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, resource, 32, 64);
    }
    func_ov005_02067468(ARCHIVE, 0, data_ov009_0207e054[2], 1);
    if (!gSaveData[1301]) {
        WORK.owned_00 = Overlay5Archive_ReadEntry(ARCHIVE, 0, 3, 0, 0);
        WORK.owned_04 = Overlay5Archive_ReadEntry(ARCHIVE, 0, 4, 0, 0);
    } else {
        WORK.owned_00 = Overlay5Archive_ReadEntry(ARCHIVE, 0, 8, 0, 0);
        WORK.owned_04 = Overlay5Archive_ReadEntry(ARCHIVE, 0, 4, 0, 0);
    }
    func_ov005_02067424(ARCHIVE, 0);
    *(void **)(WORK.font_slots + 0) = WORK.owned_00;
    *(void **)(WORK.font_slots + 4) = WORK.owned_04;
    *(void **)(WORK.font_slots + 48) = WORK.owned_00;
    *(void **)(WORK.font_slots + 52) = WORK.owned_04;
    WORK.owned_58[0] = func_ov005_0206687c(data_ov009_0207e054[3], 0, 0);
    func_ov005_02067468(ARCHIVE, 2, data_ov009_0207e054[0], 1);
    {
        void *asset_table = Overlay5Archive_ReadEntry(ARCHIVE, 2, 0, 0, 1);
        int asset_index;
        for (asset_index = 0; asset_index < 6; ++asset_index) {
            Overlay5Asset *asset = func_ov005_02066a7c(ARCHIVE, 2, (u16)assets.entries[asset_index][0],
                                                       asset_table, (u8)assets.entries[asset_index][1], 0);
            func_ov005_020698c8(asset_index, asset);
        }
        if (asset_table)
            GameHeap_DeleteArray(asset_table);
    }
    func_ov005_020671e4(ARCHIVE);
}
}
