#ifndef PIT_MENU_RESOURCE_INTERNAL_H
#define PIT_MENU_RESOURCE_INTERNAL_H
#include <game/save_menu.h>
#include <game/heap.h>
#include <game/save_data.h>
#include <game/sprite_output.h>
#include <game/overlay005_archive.h>
#include <game/overlay005_display.h>
typedef struct MenuAssetDescriptor {
    u32 entry, mode;
} MenuAssetDescriptor;
typedef struct MenuAssetTable {
    MenuAssetDescriptor assets[3];
} MenuAssetTable;
typedef struct MenuLanguageEntries {
    u32 entries[6];
} MenuLanguageEntries;
typedef struct MenuResourceWork {
    u8 unknown[444];
    void *owned[3];
    u8 unknown_1c8[0x2808 - 456];
} MenuResourceWork;
extern MenuResourceWork data_ov008_02078290;
extern SaveMenuText *data_ov008_0207828c;
extern GameSpriteAllocation data_ov005_0206a1b0, data_ov005_0206a1c8;
extern GameSpritePalette data_ov005_0206a1e0, data_ov005_0206a1f4;
extern u8 data_ov005_0206a180[];
#define ARCHIVE (*(Overlay5Archive **)(data_ov005_0206a180 + 44))
extern const MenuAssetTable data_ov008_02078044;
extern const MenuLanguageEntries data_ov008_0207802c;
extern const void *data_ov008_02077ffc[3];
extern void MI_CpuFill8(void *, u8, u32);
extern int func_02007ebc(GameSpritePalette *, int, int, int, int, int, int, int, int, int);
extern void func_ov005_02067468(Overlay5Archive *, u8, const void *, int);
extern void *Overlay5Archive_ReadEntry(Overlay5Archive *, u8, int, u32 *, int);
extern void *func_ov005_02066f78(Overlay5Archive *, int, u16, u32 *, int);
extern Overlay5Asset *func_ov005_02066a7c(Overlay5Archive *, u16, u16, const void *, u8, int);
extern void func_ov005_020698c8(u32, Overlay5Asset *);
extern void func_ov005_020671e4(Overlay5Archive *);
extern void func_0203b7a0(u32, void *, u32);
extern void DC_FlushRange(const void *, u32);
extern void func_02038984(void), func_020387b0(void), func_02038768(void), func_0203869c(void);
extern void GX_LoadTex(const void *, u32, u32), func_020386f4(const void *, u32, u32);

extern const MenuAssetDescriptor data_ov008_020781e0;
extern const void *data_ov008_020781d8[2];
typedef char MenuAssetDescriptorSizeCheck[sizeof(MenuAssetDescriptor) == 8 ? 1 : -1];
typedef char MenuAssetTableSizeCheck[sizeof(MenuAssetTable) == 24 ? 1 : -1];
typedef char MenuLanguageEntriesSizeCheck[sizeof(MenuLanguageEntries) == 24 ? 1 : -1];
typedef char MenuResourceWorkSizeCheck[sizeof(MenuResourceWork) == 0x2808 ? 1 : -1];
#endif
