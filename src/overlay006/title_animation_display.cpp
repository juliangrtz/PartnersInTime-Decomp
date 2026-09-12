#include "title_animation_internal.h"
#include <game/palette_animation.h>
#include <game/texture_allocation.h>
extern "C" {
#include <game/heap.h>
#include <game/input.h>
#include <game/sprite_output.h>
#include <nitro/gx_init.h>
void func_0202cbd4(void *, int, unsigned);
void func_02035aec(int), func_0203613c(int), func_02035a40(int), func_020359c4(int);
void func_02035b0c(int), func_02035c00(int), func_02035ebc(int), func_02035938(int), func_020358ac(int);
u32 func_02035610(void);
void func_02037190(void), func_02037108(void), VBlankIntrWait(void);
void func_02036f24(int, int, int, int);
void func_02036ee0(int, int, int, int, int);
void func_02036988(vu32 *, int, int, int, int);
void func_02036d70(u32);
void func_02009078(int, u8 *, u8 *);
}
#define REG16(address) (*(vu16 *)(address))
#define REG32(address) (*(vu32 *)(address))

/* Display buffers extend the known controller prefix, not its full allocation. */
struct TitleAnimationDisplayPrefix {
    TitleAnimationController controller;
    u32 unknown_424;
    u8 sub_extended_palette[8192];
    u8 sub_standard_palette[512];
    u8 texture_palette[32768];
};
typedef char TitleAnimationDisplayPrefix_Size[sizeof(TitleAnimationDisplayPrefix) == 42536 ? 1 : -1];

extern "C" void TitleAnimation_InitDisplay(TitleAnimationController *work)
{
    TitleAnimationDisplayPrefix *display = (TitleAnimationDisplayPrefix *)work;
    /* Clear the mapped VRAM, palettes and OAM before enabling either screen. */
    GX_DispOff();
    REG32(0x04001000) &= ~0x10000;
    REG16(0x04000304) |= 0x8000;
    func_02035aec(511);
    func_0202cbd4((void *)0x06800000, 0, 0xA4000);
    func_02035610();
    func_0202cbd4((void *)0x05000000, 0, 1024);
    func_0202cbd4((void *)0x05000400, 0, 1024);
    GameDisplay_ClearOam();
    GameDisplay_CopyOam(3);
    /* Set the geometry defaults and assign each VRAM bank its title role. */
    func_02037190();
    REG32(0x04000540) = 1;
    REG16(0x04000060) &= ~0x3002;
    REG16(0x04000060) &= 0xCFFB;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 8;
    REG16(0x04000060) = (REG16(0x04000060) & ~0x3000) | 16;
    REG16(0x04000060) &= 0xCFDF;
    func_02036f24(0, 0, 0, 0);
    func_02036ee0(0, 0, 0x7FFF, 63, 0);
    func_02037108();
    REG32(0x04000580) = 0xBFFF0000;
    GameGraphics_SetOrthographicProjection(0, -4096);
    func_02035c00(1);
    func_0203613c(2);
    func_02035a40(4);
    func_020359c4(8);
    func_02035b0c(16);
    func_02035ebc(96);
    func_02035938(128);
    func_020358ac(256);
    /* Configure main/sub backgrounds, priorities, blending and scroll offsets. */
    GX_SetGraphicsMode(1, 0, 1);
    REG32(0x04000000) = (REG32(0x04000000) & 0xFFCFFFEF) | 0x200010;
    REG32(0x04000000) &= ~0x07000000;
    REG32(0x04000000) &= ~0x38000000;
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1F00) | 0x700;
    REG16(0x04000008) &= ~3;
    REG16(0x0400000C) = (REG16(0x0400000C) & ~3) | 1;
    REG16(0x0400000E) = (REG16(0x0400000E) & ~3) | 2;
    REG16(0x0400000A) = (REG16(0x0400000A) & ~3) | 3;
    func_02036988((vu32 *)0x04000050, 5, 58, 12, 16);
    REG16(0x0400000A) = (REG16(0x0400000A) & 0x43) | 0xD88;
    REG16(0x0400000C) = (REG16(0x0400000C) & 0x43) | 0xE00;
    GXS_SetGraphicsMode(0);
    REG32(0x04001000) = (REG32(0x04001000) & 0xFFCFFFEF) | 0x200010;
    REG32(0x04001000) = (REG32(0x04001000) & ~0x1F00) | 0x1600;
    REG16(0x0400100C) = (REG16(0x0400100C) & ~3) | 1;
    REG16(0x0400100E) = (REG16(0x0400100E) & ~3) | 2;
    REG16(0x0400100A) = (REG16(0x0400100A) & ~3) | 3;
    func_02036988((vu32 *)0x04001050, 4, 59, 12, 16);
    REG16(0x0400100A) = (REG16(0x0400100A) & 0x43) | 0xD88;
    REG16(0x0400100C) = (REG16(0x0400100C) & 0x43) | 0xE00;
    func_02036d70(0);
    REG32(0x04000014) = 0;
    REG32(0x04000018) = 0;
    REG32(0x0400001C) = 0;
    REG32(0x04001010) = 0;
    REG32(0x04001014) = 0;
    REG32(0x04001018) = 0;
    REG32(0x0400101C) = 0;
    func_02009078(0, 0, 0);
    func_02009078(1, display->sub_extended_palette, display->sub_standard_palette);
    GameTexturePalette_SetBuffer(display->texture_palette, 0x8000);
    GameDisplay_SetBackgroundColor(31, 16927);
    GamePaletteEffectController *main_effects = (GamePaletteEffectController *)GameHeap_New(12, 1, 0, 0);
    if (main_effects)
        main_effects = GamePaletteEffects_Init(main_effects, 0, work->palette_entries[0], 2);
    work->palette_effects[0] = main_effects;
    GamePaletteEffectController *sub_effects = (GamePaletteEffectController *)GameHeap_New(12, 1, 0, 0);
    if (sub_effects)
        sub_effects = GamePaletteEffects_Init(sub_effects, 1, work->palette_entries[1], 2);
    work->palette_effects[1] = sub_effects;
    GXi_SetMasterBrightness((vu16 *)0x0400006C, 16);
    GXi_SetMasterBrightness((vu16 *)0x0400106C, 16);
    VBlankIntrWait();
    GX_DispOn();
    REG32(0x04001000) |= 0x10000;
}
