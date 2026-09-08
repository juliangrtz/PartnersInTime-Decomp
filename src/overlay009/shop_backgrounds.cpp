#include "shop_scene_internal.h"
extern "C" {
typedef struct ShopBackgroundEntry {
    u16 entry, unused[2];
} ShopBackgroundEntry;
typedef struct ShopBackgroundVariants {
    ShopBackgroundEntry variants[3];
} ShopBackgroundVariants;
typedef char ShopBackgroundEntrySizeCheck[sizeof(ShopBackgroundEntry) == 6 ? 1 : -1];
typedef char ShopBackgroundVariantsSizeCheck[sizeof(ShopBackgroundVariants) == 18 ? 1 : -1];
extern ShopBackgroundVariants data_ov009_0207e170[], data_ov009_0207e172[], data_ov009_0207e174[];
extern ShopBackgroundEntry data_ov009_0207e0dc[], data_ov009_0207e0de[], data_ov009_0207e0e0[];
extern void *func_ov005_020670b4(Overlay5Archive *, u8, int, u32 *, int);
extern void DC_FlushRange(const void *, u32), MI_UncompressLZ16(const void *, void *);
#define WORK data_ov009_0207ea3c
void ShopScene_LoadBackgrounds(ShopSceneTask *task)
{
    void *resource;
    if (!WORK.special_shop) {
        resource = func_ov005_020670b4(
            ARCHIVE, 1, data_ov009_0207e170[WORK.variant].variants[WORK.animated_background].entry, 0, 0);
        Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_MAIN, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(
            ARCHIVE, 1, data_ov009_0207e172[WORK.variant].variants[WORK.animated_background].entry, 0, 0);
        Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_MAIN, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(
            ARCHIVE, 1, data_ov009_0207e174[WORK.variant].variants[WORK.animated_background].entry, 0, 0);
        DC_FlushRange(resource, 512);
        Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, resource, 16384, 512);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, resource, 24576, 512);
        Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(ARCHIVE, 1, data_ov009_0207e0dc[WORK.animated_background].entry, 0, 0);
        Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_SUB, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(ARCHIVE, 1, data_ov009_0207e0de[WORK.animated_background].entry, 0, 0);
        Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_SUB, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(ARCHIVE, 1, data_ov009_0207e0e0[WORK.animated_background].entry, 0, 0);
        DC_FlushRange(resource, 512);
        Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_SUB);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_SUB, resource, 16384, 512);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_SUB, resource, 24576, 512);
        Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_SUB);
        if (resource)
            GameHeap_DeleteArray(resource);
        if (WORK.animated_background) {
            resource = func_ov005_020670b4(ARCHIVE, 1, 78, 0, 0);
            Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_SUB, 0, resource, 0);
            if (resource)
                GameHeap_DeleteArray(resource);
            resource = func_ov005_020670b4(ARCHIVE, 1, 79, 0, 0);
            MI_UncompressLZ16(resource, WORK.unknown_a0 + 0x16);
            if (resource)
                GameHeap_DeleteArray(resource);
            resource = func_ov005_020670b4(ARCHIVE, 1, 80, 0, 0);
            ((u8 *)resource)[0] = 0;
            ((u8 *)resource)[1] = 0;
            Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, resource, 0, 32);
            if (resource)
                GameHeap_DeleteArray(resource);
        }
    } else {
        resource = func_ov005_020670b4(
            ARCHIVE, 1, data_ov009_0207e170[WORK.variant].variants[WORK.animated_background].entry, 0, 0);
        Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_SUB, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(
            ARCHIVE, 1, data_ov009_0207e172[WORK.variant].variants[WORK.animated_background].entry, 0, 0);
        Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_SUB, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(
            ARCHIVE, 1, data_ov009_0207e174[WORK.variant].variants[WORK.animated_background].entry, 0, 0);
        DC_FlushRange(resource, 512);
        Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_SUB);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_SUB, resource, 16384, 512);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_SUB, resource, 24576, 512);
        Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_SUB);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(ARCHIVE, 1, data_ov009_0207e0dc[3].entry, 0, 0);
        Overlay5DisplayBg_LoadCharactersLZ(DISPLAY_ENGINE_MAIN, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(ARCHIVE, 1, data_ov009_0207e0dc[3].unused[0], 0, 0);
        Overlay5DisplayBg_LoadScreenLZ(DISPLAY_ENGINE_MAIN, 3, resource, 0);
        if (resource)
            GameHeap_DeleteArray(resource);
        resource = func_ov005_020670b4(ARCHIVE, 1, data_ov009_0207e0dc[3].unused[1], 0, 0);
        DC_FlushRange(resource, 512);
        Overlay5Display_BeginLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, resource, 16384, 512);
        Overlay5Display_LoadBgExtPalette(DISPLAY_ENGINE_MAIN, resource, 24576, 512);
        Overlay5Display_EndLoadBgExtPalette(DISPLAY_ENGINE_MAIN);
        if (resource)
            GameHeap_DeleteArray(resource);
    }
}
}
