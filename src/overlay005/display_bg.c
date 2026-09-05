#include <game/overlay005_display.h>

enum {
    REG_DISPCNT = 0x04000000,
    REG_BG_CONTROL = 0x04000008,
    REG_BG_OFFSET = 0x04000010,
    REG_BG_VERTICAL_OFFSET = 0x04000012,
    MAIN_BG_PALETTE = 0x05000000,
    MAIN_OBJ_PALETTE = 0x05000200,
    SUB_BG_PALETTE = 0x05000400,
    SUB_OBJ_PALETTE = 0x05000600,
    MAIN_BG_VRAM = 0x06000000,
    MAIN_OBJ_VRAM = 0x06400000,
};

enum {
    BG_CONTROL_PRIORITY_MASK = 0x0003,
    BG_CONTROL_CHARACTER_BASE_MASK = 0x003C,
    BG_CONTROL_MOSAIC = 0x0040,
    BG_CONTROL_COLOR_MODE = 0x0080,
    BG_CONTROL_MODE_MASK = 0x00BC,
    BG_CONTROL_SCREEN_BASE_MASK = 0x1F00,
    BG_CONTROL_OVERFLOW = 0x2000,
    BG_CONTROL_SCREEN_SIZE_MASK = 0xC000,
};

/* Metrowerks emits C functions in reverse source order. */
void *Overlay5DisplayBg_GetScreenVram2K(DisplayEngine engine, int background) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    int base_block = 0x800 * ((*control & BG_CONTROL_SCREEN_BASE_MASK) >> 8);
    int engine_offset = 0x10000 * ((*(vu32 *)REG_DISPCNT & 0x38000000) >> 27);

    return (void *)((engine << 21) + MAIN_BG_VRAM + engine_offset + base_block);
}

void *Overlay5DisplayBg_GetScreenVram16K(DisplayEngine engine, int background) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    int base_block = 0x4000 * ((*control & BG_CONTROL_SCREEN_BASE_MASK) >> 8);
    int engine_offset = 0x10000 * ((*(vu32 *)REG_DISPCNT & 0x38000000) >> 27);

    return (void *)((engine << 21) + MAIN_BG_VRAM + engine_offset + base_block);
}

void *Overlay5DisplayBg_GetCharacterVram(DisplayEngine engine, int background) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    int base_block = 0x4000 * ((*control & BG_CONTROL_CHARACTER_BASE_MASK) >> 2);
    int engine_offset = 0x10000 * ((*(vu32 *)REG_DISPCNT & 0x07000000) >> 24);

    return (void *)((engine << 21) + MAIN_BG_VRAM + engine_offset + base_block);
}

void *Overlay5Display_GetObjVram(DisplayEngine engine) {
    return (void *)((engine << 21) + MAIN_OBJ_VRAM);
}

void *Overlay5Display_GetBgPalette(DisplayEngine engine) {
    if (engine != DISPLAY_ENGINE_MAIN) {
        return (void *)SUB_BG_PALETTE;
    }
    return (void *)MAIN_BG_PALETTE;
}

void *Overlay5Display_GetObjPalette(DisplayEngine engine) {
    if (engine != DISPLAY_ENGINE_MAIN) {
        return (void *)SUB_OBJ_PALETTE;
    }
    return (void *)MAIN_OBJ_PALETTE;
}

vu16 *Overlay5DisplayBg_GetControl(DisplayEngine engine, int background) {
    return (vu16 *)(REG_BG_CONTROL + (engine << 12) + (background << 1));
}

void Overlay5DisplayBg_ClearControl(DisplayEngine engine, int background) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control = 0;
}

void Overlay5DisplayBg_SetPriority(DisplayEngine engine, int background, int priority) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_PRIORITY_MASK;
    *control |= (u16)priority;
}

void Overlay5DisplayBg_SetMosaic(DisplayEngine engine, int background, int enabled) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    if (enabled) {
        *control |= BG_CONTROL_MOSAIC;
    } else {
        *control &= ~BG_CONTROL_MOSAIC;
    }
}

void Overlay5DisplayBg_SetScreenSize(DisplayEngine engine, int background, u16 size) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_SCREEN_SIZE_MASK;
    *control |= size << 14;
}

void Overlay5DisplayBg_SetColorMode(DisplayEngine engine, int background, u16 mode) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_COLOR_MODE;
    *control |= mode << 7;
}

void Overlay5DisplayBg_SetModeBits(DisplayEngine engine, int background, u16 mode_bits) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_MODE_MASK;
    *control |= mode_bits;
}

void Overlay5DisplayBg_SetScreenBase(DisplayEngine engine, int background, u16 block) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_SCREEN_BASE_MASK;
    *control |= block << 8;
}

void Overlay5DisplayBg_SetCharacterBase(DisplayEngine engine, int background, u16 block) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_CHARACTER_BASE_MASK;
    *control |= block << 2;
}

void Overlay5DisplayBg_SetAreaOverflow(DisplayEngine engine, int background, u16 enabled) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_OVERFLOW;
    *control |= enabled << 13;
}

void Overlay5DisplayBg_SetOverflow(DisplayEngine engine, int background, u16 enabled) {
    vu16 *control = Overlay5DisplayBg_GetControl(engine, background);
    *control &= ~BG_CONTROL_OVERFLOW;
    *control |= enabled << 13;
}

void Overlay5DisplayBg_SetOffset(DisplayEngine engine, int background, s16 x, s16 y) {
    *(vu32 *)(REG_BG_OFFSET + (engine << 12) + (background << 2)) =
        (x & 0x1FF) | ((y & 0x1FF) << 16);
}

void Overlay5DisplayBg_SetVerticalOffset(DisplayEngine engine, int background, s16 y) {
    *(vu16 *)(REG_BG_VERTICAL_OFFSET + (engine << 12) + (background << 2)) = y & 0x1FF;
}
