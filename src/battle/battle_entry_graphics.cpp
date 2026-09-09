extern "C" {
#include <game/battle_entry.h>
#include <game/battle_frame.h>
#include <game/battle_context.h>
#include <game/save_data.h>
#include <game/graphics_resource.h>
#include <game/random.h>
#include <nitro/gx_init.h>
extern u8 *gBattleSystem;
extern MtxFx44 data_ov002_020bea14;
extern MtxFx44 data_ov002_020be9d4;
extern u8 data_ov002_020be9b4[];
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
void func_02035c00(int);
void func_020359c4(int);
void func_0203613c(int);
void func_02035b0c(int);
void func_02035aec(int);
void func_020358ac(int);
void func_02036988(vu32 *, int, int, int, int);
void func_02036ee0(int, int, int, int, int);
void func_02036f24(int, int, int, int);
void func_02036cc0(const MtxFx44 *);
void *func_ov002_02072508(void (*)(void), void *, int, int);
void func_ov002_020b6038(void);
}
#define REG32(address) (*(vu32 *)(address))
#define REG16(address) (*(vu16 *)(address))
#define FRAME ((BattleFrameContextView *)gBattleContext)
#define ENTRY ((BattleEntrySaveView *)(gSaveData + 0x558))
#define DISPLAY_FLAGS (&ENTRY->flags)

/* Function order follows MWCC reverse emission. */
extern "C" void BattleEntry_ConfigureVideoBanks(void)
{
    GX_ResetBankForBG();
    GX_ResetBankForOBJ();
    GX_ResetBankForTex();
    GX_ResetBankForTexPltt();
    GX_ResetBankForSubBG();
    GX_ResetBankForSubOBJ();
    func_02035610();
    GX_ResetBankForBGExtPltt();
    GX_ResetBankForOBJExtPltt();
    GX_ResetBankForSubBGExtPltt();
    GX_ResetBankForSubOBJExtPltt();
    func_02035c00(7);
    func_020359c4(8);
    func_0203613c(16);
    func_02035b0c(96);
    func_02035aec(128);
    func_020358ac(256);
    FRAME->texture_banks[0] = 0x06800000;
    FRAME->texture_banks[1] = 0x06820000;
    FRAME->palette_banks[0] = 0x06890000;
    FRAME->palette_banks[1] = 0x06894000;
    *(const u32 **)(gBattleSystem + 3576) = FRAME->texture_banks;
    GX_SetGraphicsMode(1, 0, 1);
    GXS_SetGraphicsMode(0);
    REG32(0x04000000) = REG32(0x04000000) & 0xF8FFFFFF;
    REG32(0x04000000) = REG32(0x04000000) & 0xC7FFFFFF;
    REG16(0x0400000A) = (REG16(0x0400000A) & 0x43) | 0x5A00;
    REG16(0x0400000C) = (REG16(0x0400000C) & 0x43) | 0x5C00;
    REG16(0x0400000E) = (REG16(0x0400000E) & 0x43) | 0x5E00;
    REG32(0x04000000) = (REG32(0x04000000) & 0xFFCFFFEF) | 0x10;
    REG16(0x04000008) = REG16(0x04000008) & ~3;
    REG16(0x0400000A) = (REG16(0x0400000A) & ~3) | 1;
    REG16(0x0400000C) = (REG16(0x0400000C) & ~3) | 2;
    REG16(0x0400000E) = (REG16(0x0400000E) & ~3) | 3;
    REG32(0x04000010) = 0;
    func_02036988((vu32 *)0x04000050, 1, 63, 16, 16);
    REG16(0x04001008) = (REG16(0x04001008) & 0x43) | 0x18;
    REG16(0x0400100A) = (REG16(0x0400100A) & 0x43) | 0x4204;
    REG16(0x0400100C) = (REG16(0x0400100C) & 0x43) | 0x4404;
    REG16(0x0400100E) = (REG16(0x0400100E) & 0x43) | 0x4604;
    REG32(0x04001000) = (REG32(0x04001000) & 0xFFCFFFEF) | 0x200010;
    REG16(0x04001008) = REG16(0x04001008) & ~3;
    REG16(0x0400100A) = (REG16(0x0400100A) & ~3) | 1;
    REG16(0x0400100C) = (REG16(0x0400100C) & ~3) | 2;
    REG16(0x0400100E) = (REG16(0x0400100E) & ~3) | 3;
    func_02036988((vu32 *)0x04001050, 1, 63, 16, 16);
    FRAME->render_values_30[1] = 16;
    FRAME->render_values_30[0] = 16;
    FRAME->render_values_30[3] = 1;
    FRAME->render_values_30[2] = 63;
    REG32(0x04001010) = 0;
    func_02036ee0(0, 0, 32767, 63, 0);
    REG16(0x04000060) = REG16(0x04000060) & ~0x3002;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 0x10;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    REG16(0x04000060) = REG16(0x04000060) & 0xCFFB;
    REG16(0x04000060) = REG16(0x04000060) & 0xCFDF;
    func_02036f24(0, 0, 0, 0);
    REG32(0x04000580) = 0xBFFF0000;
    REG32(0x04000540) = 1;
    GameGraphics_SetOrthographicProjection(0, -8192);
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1F00) | 0x100;
    REG32(0x04001000) = (REG32(0x04001000) & ~0x1F00) | 0x1000;
    GXi_SetMasterBrightness((vu16 *)0x0400006C, -16);
    GXi_SetMasterBrightness((vu16 *)0x0400106C, 0);
    if (DISPLAY_FLAGS->sub_screen)
        REG16(0x04000304) &= ~0x8000;
    else
        REG16(0x04000304) |= 0x8000;
}

extern "C" void BattleEntry_InitializeGraphics(void)
{
    /* Prime the 3D pipeline with the original textured quad command sequence. */
    REG32(0x040004A4) = 0x001F00C0;
    REG32(0x040004A8) = 0x5EDC8000;
    data_ov002_020bea14._31 = 24576;
    REG32(0x04000440) = 2;
    func_02036cc0(&data_ov002_020bea14);
    REG32(0x04000440) = 3;
    REG32(0x04000444) = 0;
    REG32(0x04000480) = 0x7FFF;
    func_02036cc0(&data_ov002_020be9d4);
    REG32(0x04000500) = 1;
    REG32(0x04000488) = 0xFD00FC00;
    REG32(0x0400048C) = 0xD000C000;
    REG32(0x0400048C) = 0;
    REG32(0x04000488) = 0xFD000400;
    REG32(0x0400048C) = 0xD0004000;
    REG32(0x0400048C) = 0;
    REG32(0x04000488) = 0x03000400;
    REG32(0x0400048C) = 0x2F804000;
    REG32(0x0400048C) = 0;
    REG32(0x04000488) = 0x0300FC00;
    REG32(0x0400048C) = 0x2F80C000;
    REG32(0x0400048C) = 0;
    REG32(0x04000504) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000448) = 1;
    REG32(0x04000454) = 0;
    /* One fixed sixteen-color palette and the live 256-color battle palette. */
    GameTexturePalette_Allocate((GameTexturePalette *)(gBattleContext + 26844), 2, 3, 16, 0, 1, 0,
                                data_ov002_020be9b4, 0, 0);
    GameTexturePalette_Allocate((GameTexturePalette *)(gBattleContext + 52100), 2, 4, 256, 0, 0, 0,
                                FRAME->main_palette, 0x3021, 32);
    BattleAI_Initialize();
    func_ov002_020b6038();
    func_ov002_02072508(BattleEntry_ShowBattleDisplay, 0, 0, 0);
    FRAME->main_brightness_level = 32;
    FRAME->sub_brightness_level = 0;
    FRAME->vblank = BattleMain_VBlank;
    FRAME->update = BattleEntry_WaitArchives;
}

extern "C" void BattleEntry_ShowBattleDisplay(void)
{
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1F00) | 0x100;
    REG32(0x04001000) = REG32(0x04001000) & ~0x1F00;
    GXi_SetMasterBrightness((vu16 *)0x0400006C, 0);
    GXi_SetMasterBrightness((vu16 *)0x0400106C, -16);
    if (DISPLAY_FLAGS->sub_screen)
        REG16(0x04000304) |= 0x8000;
    else
        REG16(0x04000304) &= ~0x8000;
}

extern "C" void BattleEntry_SeedRandomFromClock(u32 result, void *argument)
{
    Random_SetSeed(FRAME->clock.second + (60 * FRAME->clock.minute + 3600 * FRAME->clock.hour));
}
