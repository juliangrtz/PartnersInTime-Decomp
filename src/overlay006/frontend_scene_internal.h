#ifndef PIT_FRONTEND_SCENE_INTERNAL_H
#define PIT_FRONTEND_SCENE_INTERNAL_H
#include <game/frontend_scene.h>
#include <game/save_data.h>
#include <game/save_storage.h>
#include <game/heap.h>
#include <game/audio.h>
#include <game/input.h>
#include <game/sprite_output.h>
#include <game/overlay005_display.h>
#include <game/overlay005_archive.h>
#include <nitro/gx_init.h>
#include <nitro/gx_projection.h>
#define REG16(address) (*(vu16 *)(address))
#define REG32(address) (*(vu32 *)(address))
#define MENU_ARCHIVE (*(Overlay5Archive **)(data_ov005_0206a180 + 44))
#define MENU_OAM_BUFFERS (*(u8 **)(data_ov005_0206a180 + 40))
#define ARCHIVE MENU_ARCHIVE
typedef struct MenuElement MenuElement;
typedef struct CreditsLanguageEntries {
    u32 entries[6];
} CreditsLanguageEntries;
typedef struct TitleResourceSet {
    u16 characters, screen, palette;
} TitleResourceSet;
typedef struct TitleResourceTable {
    TitleResourceSet sets[2];
} TitleResourceTable;
typedef struct CreditsResourceEntry {
    u16 entry, unused;
} CreditsResourceEntry;
/* Transition fields followed by the credits' remaining private workspace. */
typedef struct CreditsWorkspace {
    u8 unknown_00[28];
    int layout, variant, screen, active;
    u32 unknown_2c;
    int polygon_alpha;
    u8 unknown_34[0x8208];
} CreditsWorkspace;
typedef char CreditsWorkspaceSizeCheck[sizeof(CreditsWorkspace) == 0x823c ? 1 : -1];
typedef struct SaveDirectoryView {
    u8 unknown[8];
    u8 selected : 4;
    u8 present : 4;
} SaveDirectoryView;
extern u8 data_0206032c[];
extern u8 data_ov005_0206a180[];
extern void func_ov005_02066128(int);
extern void func_ov005_0206781c(int, int);
extern void func_ov005_02066638(int);
extern void func_ov005_020690f0(int);
extern void func_ov005_02069b54(void);
extern void func_ov005_02068ca8(int);
extern void func_ov005_02069674(int);
extern void func_ov005_02069998(int);
extern Overlay5Archive *func_ov005_02067584(Overlay5Archive *, int, int, void *, int);
extern MenuElement *func_ov005_0206659c(void (*)(MenuElement *), int, int);
extern void func_02009078(int, void *, void *);
extern void func_02036988(vu16 *, int, int, int, int);
extern void func_02037190(void);
extern void func_02037024(void);
extern void func_02037108(void);
extern void func_02036ee0(int, int, int, int, int);
extern void func_02035fd0(int);
extern void func_0203613c(int);
extern void func_02035ebc(int);
extern void func_02035c00(int);
extern void func_02035b0c(int);
extern void func_020359c4(int);
extern void func_02035a40(int);
extern void func_02035938(int);
extern void func_02035aec(int);
extern GameTaskVTable data_ov006_0207b51c;
extern TitleSceneTask *data_ov006_0207c4e8;
extern GameTaskVTable data_ov006_0207bc1c;
extern CreditsSceneTask *data_ov006_0207c4f0;
extern void TitleStartup_UpdateGreeting(MenuElement *);
extern void func_ov006_02077d5c(MenuElement *);
extern void func_ov005_020671e4(Overlay5Archive *);
extern void func_ov005_02067424(Overlay5Archive *, u8);
extern void func_ov005_02065f10(void);
extern void func_02009058(int);
extern void func_ov005_02069928(void);
extern void func_ov005_02069660(void);
extern void func_ov005_02068c94(void);
extern void func_ov005_020699f0(void);
extern void func_ov005_020690dc(void);
extern void func_ov005_02066624(void);
extern int func_ov005_02065f00(void);
extern void func_ov005_0206651c(int, int);
extern void func_ov005_0206642c(int, int);
extern void func_ov005_02068f84(int);
extern void func_ov005_02065eec(void);
extern GameTaskVTable data_ov006_0207b508;
extern GameTaskVTable data_ov006_0207bba4;
extern void func_ov005_02067468(Overlay5Archive *, u8, const void *, int);
extern void *Overlay5Archive_ReadEntry(Overlay5Archive *, u8, int, u32 *, int);
extern void *func_ov005_02066f78(Overlay5Archive *, int, u16, u32 *, int);
extern Overlay5Asset *func_ov005_02066a7c(Overlay5Archive *, u16, u16, const void *, u8, int);
extern void func_ov005_020698c8(u32, Overlay5Asset *);
extern void DC_FlushRange(const void *, u32);
extern void func_02038984(void);
extern void GX_LoadTex(const void *, u32, u32);
extern void func_020387b0(void);
extern void func_02038768(void);
extern void func_020386f4(const void *, u32, u32);
extern void func_0203869c(void);
extern void MI_CpuFill8(void *, u8, u32);
extern void MI_UncompressLZ16(const void *, void *);
extern SaveDirectoryView data_0205e32c;
extern int data_ov006_0207c4ec;
extern const void *data_ov006_0207b490[2];
extern const TitleResourceTable data_ov006_0207b4a4;
extern const void *data_ov006_0207b5e4[3];
extern const CreditsLanguageEntries data_ov006_0207bd00, data_ov006_0207bd18;
extern CreditsWorkspace data_ov006_0207c594;
extern s8 data_ov006_0207bfe8;
extern CreditsResourceEntry data_ov006_0207bed8[], data_ov006_0207beda[];

#endif
