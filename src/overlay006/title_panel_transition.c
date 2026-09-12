#include <game/title_panel_transition.h>
#include <nitro.h>
#include <nitro/gx_init.h>
#include <game/heap.h>
#include <game/input.h>

#define REG16(address) (*(vu16 *)(address))
#define REG32(address) (*(vu32 *)(address))
extern const s16 FX_SinCosTable_[];
extern void *data_ov006_0207c4e4;
void *func_ov006_0206b77c(void *, int, int, int, int, int, u32 *);
void func_02036988(vu16 *, int, int, int, int);
void func_02037fd0(const void *, u32, u32), func_02037f68(const void *, u32, u32);
void func_02038310(const void *, u32, u32), func_020382a8(const void *, u32, u32);
void func_02038240(const void *, u32, u32), func_020381d8(const void *, u32, u32);
void func_020380a0(const void *, u32, u32), func_02038038(const void *, u32, u32);
void func_020383e0(const void *, u32, u32), func_02038378(const void *, u32, u32);
void GX_BeginLoadBGExtPltt(void), GXS_BeginLoadBGExtPltt(void);
void GX_LoadBGExtPltt(const void *, u32, u32), GXS_LoadBGExtPltt(const void *, u32, u32);
void GX_EndLoadBGExtPltt(void), GXS_EndLoadBGExtPltt(void);
void *func_02036724(void), *func_020366a4(void);
void func_0202cbd4(void *, int, u32);
void DC_FlushRange(const void *, u32);
u32 OS_DisableIrqMask(u32), OS_EnableIrqMask(u32);
void OS_SetIrqFunction(u32, void (*)(void));

static inline void WindowPlanes(vu16 *reg, int shift, int planes, int effects)
{
    /* WININ/WINOUT: five layer bits and one color-effect bit per window. */
    u32 value = (*reg & ~(0x3f << shift)) | (planes << shift);
    if (effects)
        value |= 0x20 << shift;
    *reg = value;
}
static inline void PanelControls(void)
{
    REG16(0x0400000c) = (REG16(0x0400000c) & 0x43) | 0xe80;
    REG16(0x0400000e) = (REG16(0x0400000e) & 0x43) | 0xf80;
    REG16(0x0400100c) = (REG16(0x0400100c) & 0x43) | 0xe80;
    REG16(0x0400100e) = (REG16(0x0400100e) & 0x43) | 0xf80;
}
static inline void LoadPanelPalette(TitlePanelTransition *work)
{
    GX_BeginLoadBGExtPltt();
    GXS_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(work->palette, 0x4000, 512);
    GX_LoadBGExtPltt(work->palette, 0x6000, 512);
    GXS_LoadBGExtPltt(work->palette, 0x4000, 512);
    GXS_LoadBGExtPltt(work->palette, 0x6000, 512);
    GX_EndLoadBGExtPltt();
    GXS_EndLoadBGExtPltt();
}

void TitleScene_LoadBackgrounds(void)
{
    u32 screen_size, character_size, palette_size;
    void *screen, *characters;
    u16 *palette;
    u16 main_color, sub_color;
    screen = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 1, 1, 0, 0, &screen_size);
    characters = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 0, 1, 0, 0, &character_size);
    palette = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 2, 0, 0, 0, &palette_size);
    DC_FlushRange(characters, character_size);
    DC_FlushRange(screen, screen_size);
    DC_FlushRange(palette, palette_size);
    func_020380a0(characters, 0, character_size);
    func_020383e0(screen, 0, screen_size);
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(palette, 0x2000, palette_size);
    GX_EndLoadBGExtPltt();
    main_color = *palette;
    if (palette) GameHeap_Free(palette);
    if (characters) GameHeap_Free(characters);
    if (screen) GameHeap_Free(screen);
    screen = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 4, 1, 0, 0, &screen_size);
    characters = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 3, 1, 0, 0, &character_size);
    palette = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 5, 0, 0, 0, &palette_size);
    DC_FlushRange(characters, character_size);
    DC_FlushRange(screen, screen_size);
    DC_FlushRange(palette, palette_size);
    func_02038038(characters, 0, character_size);
    func_02038378(screen, 0, screen_size);
    GXS_BeginLoadBGExtPltt();
    GXS_LoadBGExtPltt(palette, 0x2000, palette_size);
    GXS_EndLoadBGExtPltt();
    sub_color = *palette;
    if (palette) GameHeap_Free(palette);
    if (characters) GameHeap_Free(characters);
    if (screen) GameHeap_Free(screen);
    GameDisplay_SetBackgroundColor(main_color, sub_color);
}

void TitlePanelTransition_Update(TitlePanelTransition *work)
{
    switch (work->state) {
    case TITLE_PANEL_STEADY: break;
    case TITLE_PANEL_SETUP: break;
    case TITLE_PANEL_SLIDING:
        ++work->elapsed;
        if (work->elapsed < 40) {
            /* Preserve the native unsigned intermediate before signed Q12 truncation. */
            unsigned offset = -FX_SinCosTable_[2 * (((work->elapsed << 14) / 40 + 0x4000) >> 4)];
            work->displacement = (int)((offset + 4096) << 7) / 4096;
        } else {
            work->state = TITLE_PANEL_FINISH;
            work->displacement = 128;
        }
        break;
    case TITLE_PANEL_FINISH: break;
    }
}

void TitlePanelTransition_SetScanlineLayers(int line)
{
    if (line < 192 && line >= 128)
        REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0xf00;
    if (line < 192 && line >= 140)
        REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x1200;
}
void TitlePanelTransition_HBlank(void)
{
    TitlePanelTransition_SetScanlineLayers(REG16(0x04000006));
}
void TitlePanelTransition_EnableHBlank(void)
{
    OS_DisableIrqMask(2);
    OS_SetIrqFunction(2, TitlePanelTransition_HBlank);
    OS_EnableIrqMask(2);
    GX_HBlankIntr(1);
}
void TitlePanelTransition_DisableHBlank(void)
{
    OS_DisableIrqMask(2);
}
void TitleScene_SetPanelScroll(int displacement)
{
    unsigned left = (128 - displacement) & 511;
    unsigned right = (displacement - 128) & 511;
    REG32(0x04000018) = left | 0x01800000;
    REG32(0x04001018) = left | 0x00740000;
    REG32(0x0400001c) = right | 0x01800000;
    REG32(0x0400101c) = right | 0x00740000;
}
void TitlePanelTransition_SetWindows(void)
{
    WindowPlanes((vu16 *)0x0400004a, 0, 3, 1);
    WindowPlanes((vu16 *)0x0400104a, 0, 0x12, 1);
    WindowPlanes((vu16 *)0x04000048, 0, 7, 1);
    WindowPlanes((vu16 *)0x04001048, 0, 0x16, 1);
    REG16(0x04000040) = 128;
    REG16(0x04000044) = 192;
    REG16(0x04001040) = 128;
    REG16(0x04001044) = 192;
    WindowPlanes((vu16 *)0x04000048, 8, 0xb, 1);
    WindowPlanes((vu16 *)0x04001048, 8, 0x1a, 1);
    REG16(0x04000042) = 0x8000;
    REG16(0x04000046) = 192;
    REG16(0x04001042) = 0x8000;
    REG16(0x04001046) = 192;
    REG32(0x04000000) = (REG32(0x04000000) & ~0xe000) | 0x6000;
    REG32(0x04001000) = (REG32(0x04001000) & ~0xe000) | 0x6000;
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0x300;
    REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x1e00;
}
void TitlePanelTransition_SetBlend(int displacement)
{
    int first = displacement * 12 / 128;
    int second = 16 + displacement * -10 / 128;
    func_02036988((vu16 *)0x04000050, 12, 50, first, second);
    func_02036988((vu16 *)0x04001050, 12, 50, first, second);
}
void TitlePanelTransition_Upload(TitlePanelTransition *work)
{
    int displacement = work->displacement;
    switch (work->state) {
    case TITLE_PANEL_STEADY:
        REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0x700;
        REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x1600;
        break;
    case TITLE_PANEL_SETUP:
        TitlePanelTransition_EnableHBlank();
        work->loaded.bits.hblank_installed = 1;
        PanelControls();
        func_02037fd0(work->characters, 0, work->character_size);
        func_02037f68(work->characters, 0, work->character_size);
        func_02038310(work->left_screen, 0, 2048);
        func_020382a8(work->left_screen, 0, 2048);
        func_02038240(work->right_screen, 0, 2048);
        func_020381d8(work->right_screen, 0, 2048);
        LoadPanelPalette(work);
        TitleScene_SetPanelScroll(0);
        TitlePanelTransition_SetWindows();
        TitlePanelTransition_SetBlend(0);
        work->loaded.value |= 2;
        work->state = TITLE_PANEL_SLIDING;
        break;
    case TITLE_PANEL_SLIDING:
        TitleScene_SetPanelScroll(work->displacement);
        TitlePanelTransition_SetWindows();
        TitlePanelTransition_SetBlend(displacement);
        break;
    case TITLE_PANEL_FINISH:
        if (work->loaded.bits.hblank_installed)
            TitlePanelTransition_DisableHBlank();
        PanelControls();
        REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0x700;
        REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x1600;
        WindowPlanes((vu16 *)0x0400004a, 0, 3, 1);
        WindowPlanes((vu16 *)0x0400104a, 0, 0x12, 1);
        WindowPlanes((vu16 *)0x04000048, 0, 7, 1);
        WindowPlanes((vu16 *)0x04000048, 8, 7, 1);
        WindowPlanes((vu16 *)0x04001048, 0, 0x16, 1);
        WindowPlanes((vu16 *)0x04001048, 8, 0x16, 1);
        REG16(0x04000040) = 128;
        REG16(0x04000044) = 0x80c0;
        REG16(0x04000042) = 0x8000;
        REG16(0x04000046) = 0x80c0;
        REG16(0x04001040) = 128;
        REG16(0x04001044) = 140;
        REG16(0x04001042) = 0x8000;
        REG16(0x04001046) = 140;
        REG32(0x04000000) = (REG32(0x04000000) & ~0xe000) | 0x6000;
        REG32(0x04001000) = (REG32(0x04001000) & ~0xe000) | 0x6000;
        func_02038310(work->final_screen, 0, 2048);
        func_020382a8(work->final_screen, 0, 2048);
        func_0202cbd4(func_02036724(), 0, 2048);
        func_0202cbd4(func_020366a4(), 0, 2048);
        if (!work->loaded.bits.assets_uploaded) {
            func_02037fd0(work->characters, 0, work->character_size);
            func_02037f68(work->characters, 0, work->character_size);
            LoadPanelPalette(work);
            work->loaded.value |= 2;
        }
        TitleScene_SetPanelScroll(128);
        TitlePanelTransition_SetBlend(128);
        work->state = TITLE_PANEL_STEADY;
        break;
    }
}
void TitlePanelTransition_Init(TitlePanelTransition *work, const void *characters, u32 size,
                       const void *left, const void *right, const void *final_screen, const void *palette)
{
    work->characters = characters;
    work->character_size = size;
    work->left_screen = left;
    work->right_screen = right;
    work->final_screen = final_screen;
    work->palette = palette;
    work->displacement = 0;
    work->unknown_42 = 0;
    work->draw_flags.bits.unknown_0 = 1;
    work->draw_flags.value |= 4;
    work->update = TitlePanelTransition_Update;
    work->upload = TitlePanelTransition_Upload;
    work->state = TITLE_PANEL_SETUP;
}
void TitlePanelTransition_Skip(TitlePanelTransition *work)
{
    work->draw_flags.value = 0;
    work->state = TITLE_PANEL_FINISH;
}
void TitlePanelTransition_Release(TitlePanelTransition *work)
{
    /* Native cleanup callback; the parent owns the shared resource buffers. */
}
