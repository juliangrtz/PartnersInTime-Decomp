#ifndef PIT_PAUSE_SCENE_INTERNAL_H
#define PIT_PAUSE_SCENE_INTERNAL_H
#include <game/pause_scene.h>
extern "C" {
#include <game/heap.h>
#include <game/save_data.h>
#include <game/input.h>
#include <game/overlay005_display.h>
#include <game/overlay005_archive.h>
#include <game/sprite_output.h>
#include <nitro/gx_init.h>
}
extern "C" {
#include <game/rumble.h>
#include <game/scene_script.h>
}
struct PausePanel {
    virtual void destroy();
    virtual void deleteInstance();
};
struct PauseSceneWork {
    u8 unknown00[0x1e];
    u16 input_mask;
    u8 unknown20[8];
    void *owned28, *owned2c;
    u8 unknown30[0x50];
    void *owned80;
    u8 unknown84, tutorial, background_dirty, unknown87;
    void *owned88, *owned8c, *owned90, *owned94;
    u8 palette98[20], paletteac[20];
    void *ownedc0;
    void *ownedc4[2], *ownedcc[2], *ownedd4[2];
    void *owneddc, *ownede0, *ownede4;
    u8 palettee8[20], palettefc[20];
    u8 unknown110[0x1ac];
    void *renderer;
    u8 unknown2c0[32];
    GameSpriteAllocation main_allocation, sub_allocation;
    void *sprites[48];
    PausePanel *panel;
    u8 unknown3d4[90600 - 0x3d4];
};
typedef char PauseSceneWorkSizeCheck[sizeof(PauseSceneWork) == 90600 ? 1 : -1];
struct PauseSavedState {
    u8 unknown[1370];
    s16 tutorial;
};
typedef char PauseSavedStateSizeCheck[sizeof(PauseSavedState) == 1372 ? 1 : -1];
extern "C" {
extern GameTaskVTable data_ov007_0208d9b8, data_ov007_0208d9a4;
extern PauseSceneTask *data_ov007_0208e1e0;
extern void *data_ov007_0208e1e4;
extern u8 *data_ov007_020a6b90;
extern PauseSceneWork data_ov007_020905f0;
extern u8 data_ov007_0208e1e8[], data_ov007_0208e02c[];
extern GameRumblePattern data_ov007_0208d738[];
extern u8 data_0205a00c;
extern void *SceneManager_LoadResources(void *);
extern void SceneManager_Shutdown(void *), SceneController_Create(u8 *), SceneTask_LoadArchiveMember(u8 *);
extern void SceneScript_LoadPrimaryArchive(u8 *, int, u16);
extern void func_ov007_02081664(int), func_ov007_02070b50(PauseSceneTask *), func_ov007_0208a918(void);
extern void func_ov007_0206e918(PauseMenuElement *), func_ov007_020762dc(void *);
extern void func_ov005_02068c54(void *);

extern void MI_StopDma(int);
#define WORK data_ov007_020905f0
#define SAVED_TUTORIAL (((PauseSavedState *)gSaveData)->tutorial)
static inline void DeleteSceneManager(void *p)
{
    if (p) {
        SceneManager_Shutdown(p);
        GameHeap_Delete(p);
    }
}
static inline void DeleteParty(void *p)
{
    if (p) {
        func_ov007_020762dc(p);
        GameHeap_Delete(p);
    }
}
static inline void DeletePausePanel(PausePanel *p)
{
    if (p)
        p->deleteInstance();
}
extern void func_ov005_02066128(int), func_ov005_0206781c(int, int);
extern void func_ov005_02066638(int), func_ov005_020690f0(int), func_ov005_02069b54(void),
    func_ov005_02068ca8(int), func_ov005_02069674(int), func_ov005_02069998(int);
extern Overlay5Archive *func_ov005_02067584(Overlay5Archive *, int, int, void *, int);
extern void func_ov005_020671e4(Overlay5Archive *), func_ov005_02067424(Overlay5Archive *, u8);
extern void func_ov005_020695cc(void *);
extern void func_02009078(int, void *, void *), func_02009058(int);
extern void func_ov005_02065f10(void), func_ov005_02069928(void), func_ov005_02069660(void),
    func_ov005_02068c94(void), func_ov005_020699f0(void), func_ov005_020690dc(void),
    func_ov005_02066624(void);
extern void func_02035fd0(int), func_0203613c(int), func_02035e04(int), func_02035ebc(int),
    func_020359c4(int), func_02035a40(int), func_020358ac(int), func_02035938(int);
extern void MI_CpuFill8(void *, u8, u32);

extern u8 data_ov005_0206a180[], data_0206032c[];
#define ARCHIVE (*(Overlay5Archive **)(data_ov005_0206a180 + 44))
#define OAM_BUFFERS (*(u8 **)(data_ov005_0206a180 + 40))
#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))
extern PauseMenuElement *func_ov005_0206659c(void (*)(PauseMenuElement *), int, int);
}
#endif
