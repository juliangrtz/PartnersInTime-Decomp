extern "C" {
#include <game/battle_entry.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
#include <game/save_data.h>
#include <game/heap.h>
#include <nitro/gx_init.h>
extern u8 data_ov002_020beb1c[];
extern u8 data_02059f58;
u32 GX_ResetBankForBG(void);
u32 GX_ResetBankForOBJ(void);
u32 GX_ResetBankForTex(void);
u32 GX_ResetBankForTexPltt(void);
u32 GX_ResetBankForSubBG(void);
u32 GX_ResetBankForSubOBJ(void);
u32 GX_ResetBankForBGExtPltt(void);
u32 GX_ResetBankForOBJExtPltt(void);
u32 GX_ResetBankForSubBGExtPltt(void);
u32 GX_ResetBankForSubOBJExtPltt(void);
u32 func_02035610(void);
void func_0203613c(int);
void func_02035aec(int);
void func_02035ebc(int);
void func_02035e04(int);
void func_02035fd0(int);
void func_0202cbd4(void *, int, u32);
int func_0202dca8(const s16 *, int, u16 *, int, const u16 *, int);
void *func_ov002_02072508(void (*)(void), void *, int, int);
void func_ov002_02075818(void);
void func_ov002_02074ad4(BattleQueuedTask *);
void GameAudio_StartStream(int);
}
#define REG32(address) (*(vu32 *)(address))
#define REG16(address) (*(vu16 *)(address))
#define FRAME ((BattleFrameContextView *)gBattleContext)
#define ENTRY ((BattleEntrySaveView *)(gSaveData + 0x558))
#define DISPLAY_FLAGS (&ENTRY->flags)

enum BattleEntryStateOffset {
    BATTLE_ENTRY_CLEAR_BEGIN = 0x10,
    BATTLE_ENTRY_CLEAR_END = 0xE164,
    BATTLE_ENTRY_BUFFER_OFFSET = 0xE160
};

static inline void CopyMemory(const void *source, void *destination, u32 size)
{
    /* Keep the native address comparison even for fixed hardware windows. */
    if (source < destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
}

/* Function order follows MWCC reverse emission. */
extern "C" int BattlePalette_Animate(const s16 *commands, int colors, const u16 *buffer, u16 **destination)
{
    if (*destination)
        buffer = 0;
    else
        *destination = (u16 *)buffer + 256;
    return func_0202dca8(commands, FRAME->frame << 8, *destination, colors, buffer, 1);
}

extern "C" void BattleMain_Initialize(void)
{
    func_0202cbd4(gBattleContext + BATTLE_ENTRY_CLEAR_BEGIN, 0,
                  (u32)(gBattleContext + BATTLE_ENTRY_CLEAR_END) -
                      (u32)(gBattleContext + BATTLE_ENTRY_CLEAR_BEGIN));
    *(void **)(gBattleContext + BATTLE_ENTRY_BUFFER_OFFSET) =
        GameHeap_Allocate(0, 0x8000, data_ov002_020beb1c, 1);
    RTC_GetTimeAsync(&FRAME->clock, BattleEntry_SeedRandomFromClock, 0);
    FRAME->initial_state_3e = data_02059f58;
    data_02059f58 = 0;
    FRAME->main_brightness_level = 32;
    BattleTaskQueue_Enqueue(func_ov002_02074ad4, 0);
    func_ov002_02072508(BattleEntry_ApplyFieldBrightness, 0, 0, 0);
    FRAME->update = BattleEntry_FadeField;
}

extern "C" void BattleEntry_ApplyFieldBrightness(void)
{
    if (DISPLAY_FLAGS->sub_screen)
        GXi_SetMasterBrightness((vu16 *)0x0400006C, FRAME->main_brightness_level / 2 - 16);
    else
        GXi_SetMasterBrightness((vu16 *)0x0400106C, FRAME->main_brightness_level / 2 - 16);
}

extern "C" void BattleEntry_FadeField(void)
{
    func_ov002_02072508(BattleEntry_ApplyFieldBrightness, 0, 0, 0);
    if (FRAME->main_brightness_level > 0)
        FRAME->main_brightness_level -= 3;
    if (FRAME->main_brightness_level <= 0) {
        FRAME->main_brightness_level = 0;
        if (DISPLAY_FLAGS->sub_screen) {
            func_ov002_02072508(BattleEntry_MapCaptureBanks, 0, 0, 0);
            FRAME->update = BattleEntry_CopySubDisplayToMain;
        } else {
            func_ov002_02072508(BattleEntry_RestoreFieldDisplay, 0, 0, 0);
            FRAME->update = BattleEntry_StartTransitionSound;
        }
    }
}

extern "C" void BattleEntry_MapCaptureBanks(void)
{
    GX_ResetBankForBG();
    GX_ResetBankForOBJ();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();
    func_0203613c(1);
    func_02035fd0(2);
}

extern "C" void BattleEntry_CopySubDisplayToMain(void)
{
    CopyMemory((void *)0x06200000, (void *)0x06000000, 0x20000);
    CopyMemory((void *)0x06600000, (void *)0x06400000, 0x20000);
    CopyMemory((void *)0x05000400, (void *)0x05000000, 1024);
    CopyMemory((void *)0x07000400, (void *)0x07000000, 1024);
    func_ov002_02072508(BattleEntry_RestoreFieldDisplay, 0, 0, 0);
    FRAME->update = BattleEntry_StartTransitionSound;
}

extern "C" void BattleEntry_RestoreFieldDisplay(void)
{
    if (DISPLAY_FLAGS->sub_screen) {
        /* The native register snapshot is materialized before writing DISPCNT. */
        volatile u32 display = (REG32(0x04000000) & 0x3F060008) | REG32(0x04001000);
        REG32(0x04000000) = display;
        REG16(0x04000008) = REG16(0x04001008);
        REG16(0x0400000A) = REG16(0x0400100A);
        REG16(0x0400000C) = REG16(0x0400100C);
        REG16(0x0400000E) = REG16(0x0400100E);
        REG16(0x04000050) = REG16(0x04001050);
        REG16(0x04000052) = REG16(0x04001052);
        REG32(0x04000000) = (REG32(0x04000000) & ~0x1F00) | 0x1F00;
        REG32(0x04001000) = REG32(0x04001000) & ~0x1F00;
        GX_ResetBankForSubBG();
        GX_ResetBankForSubOBJ();
        GX_ResetBankForSubBGExtPltt();
        GX_ResetBankForSubOBJExtPltt();
        func_02035aec(432);
        CopyMemory((void *)0x06898000, (void *)0x06880000, 0x8000);
        CopyMemory((void *)0x068A0000, (void *)0x06890000, 0x4000);
        func_02035ebc(16);
        func_02035e04(32);
        REG32(0x04000014) =
            (*(s16 *)(gSaveData + 1386) & 511) | ((*(s16 *)(gSaveData + 1398) << 16) & 0x1FF0000);
        REG32(0x04000018) =
            (*(s16 *)(gSaveData + 1388) & 511) | ((*(s16 *)(gSaveData + 1400) << 16) & 0x1FF0000);
        REG32(0x0400001C) =
            (*(s16 *)(gSaveData + 1390) & 511) | ((*(s16 *)(gSaveData + 1402) << 16) & 0x1FF0000);
        GXi_SetMasterBrightness((vu16 *)0x0400006C, 0);
        GXi_SetMasterBrightness((vu16 *)0x0400106C, -16);
        REG16(0x04000304) |= 0x8000;
    } else {
        REG32(0x04001000) = REG32(0x04001000) & ~0x1F00;
        GX_ResetBankForSubBG();
        GX_ResetBankForSubOBJ();
        GX_ResetBankForSubBGExtPltt();
        GX_ResetBankForSubOBJExtPltt();
        REG32(0x04000014) =
            (*(s16 *)(gSaveData + 1380) & 511) | ((*(s16 *)(gSaveData + 1392) << 16) & 0x1FF0000);
        REG32(0x04000018) =
            (*(s16 *)(gSaveData + 1382) & 511) | ((*(s16 *)(gSaveData + 1394) << 16) & 0x1FF0000);
        REG32(0x0400001C) =
            (*(s16 *)(gSaveData + 1384) & 511) | ((*(s16 *)(gSaveData + 1396) << 16) & 0x1FF0000);
        GXi_SetMasterBrightness((vu16 *)0x0400006C, 0);
        GXi_SetMasterBrightness((vu16 *)0x0400106C, -16);
        REG16(0x04000304) &= ~0x8000;
    }
    func_0202cbd4((void *)0x07000400, 0, 1024);
    func_02035610();
    func_02035aec(12);
}

extern "C" void BattleEntry_StartTransitionSound(void)
{
    /* Signed narrowing precedes the unsigned two-value approach check. */
    int stream;
    if (ENTRY->formation == 1) {
        if (((u16)(s16)(ENTRY->flags.approach + 2) <= 1)) {
            if (ENTRY->flags.luigi)
                stream = 34;
            else
                stream = 22;
        } else {
            if (*(s16 *)(gSaveData + 1370) == 20480)
                stream = 21;
            else
                stream = 5;
        }
    } else if (((u16)(s16)(ENTRY->flags.approach + 2) <= 1)) {
        if (ENTRY->flags.luigi)
            stream = 15;
        else
            stream = 10;
    } else {
        if (*(s16 *)(gSaveData + 1370) == 20481)
            stream = 7;
        else
            stream = 4;
    }
    GameAudio_StartStream(stream);
    /* Capture the scene for the following display transition. */
    REG32(0x04000064) = 0x80371010;
    FRAME->update = BattleEntry_CaptureScene;
}

extern "C" void BattleEntry_CaptureScene(void)
{
    REG32(0x04000064) = 0x80321010;
    func_ov002_02072508(func_ov002_02075818, 0, 0, 0);
    FRAME->update = BattleEntry_WaitCapture;
}
