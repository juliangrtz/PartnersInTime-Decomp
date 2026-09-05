#include <game/overlay005_display.h>

extern void GX_EndLoadOBJExtPltt(void);
extern void GX_LoadOBJExtPltt(const void *source, u32 offset, u32 size);
extern void GX_BeginLoadOBJExtPltt(void);
extern void GX_EndLoadBGExtPltt(void);
extern void GX_LoadBGExtPltt(const void *source, u32 offset, u32 size);
extern void GX_BeginLoadBGExtPltt(void);
extern void GXS_EndLoadOBJExtPltt(void);
extern void GXS_LoadOBJExtPltt(const void *source, u32 offset, u32 size);
extern void GXS_BeginLoadOBJExtPltt(void);
extern void GXS_EndLoadBGExtPltt(void);
extern void GXS_LoadBGExtPltt(const void *source, u32 offset, u32 size);
extern void GXS_BeginLoadBGExtPltt(void);
extern void MI_UncompressLZ16(const void *source, void *destination);
extern void MIi_CpuCopy16(const void *source, void *destination, u32 size);
extern void MI_CpuCopy8(const void *source, void *destination, u32 size);
extern void DisplayVram_ClearEngineAssignments(DisplayEngine engine);
extern void G2x_SetBGyAffine_(
    u32 address,
    const DisplayAffineMatrix *matrix,
    int center_x,
    int center_y,
    int origin_x,
    int origin_y
);
extern u32 GX_ResetBankForBG(void);
extern u32 GX_ResetBankForOBJ(void);
extern u32 GX_ResetBankForBGExtPltt(void);
extern u32 GX_ResetBankForOBJExtPltt(void);
extern u32 GX_ResetBankForTex(void);
extern u32 GX_ResetBankForTexPltt(void);
extern u32 GX_ResetBankForClearImage(void);
extern u32 GX_ResetBankForSubBG(void);
extern u32 GX_ResetBankForSubOBJ(void);
extern u32 GX_ResetBankForSubBGExtPltt(void);
extern u32 GX_ResetBankForSubOBJExtPltt(void);
extern s16 FX_SinCosTable_[];

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

enum {
    DISPCNT_VISIBLE_PLANE_MASK = 0x00001F00,
    DISPCNT_VISIBLE_WINDOW_MASK = 0x0000E000,
    DISPCNT_OBJ_TILE_MAPPING_MASK = 0x00300010,
    DISPCNT_HBLANK_OBJ_PROCESSING = 0x00800000,
    REG_BG2_AFFINE = 0x04000020,
    REG_BG3_AFFINE = 0x04000030,
};

extern u8 gOverlay5DisplayObjTileBoundary[];

/* Metrowerks emits C functions in reverse source order. */
void Overlay5Display_ResetEngine(DisplayEngine engine) {
    int background;

    DisplayVram_ClearEngineAssignments(engine);
    Overlay5Display_SetVisiblePlanes(engine, DISPLAY_PLANE_NONE);
    Overlay5Display_SetVisibleWindows(engine, DISPLAY_WINDOW_NONE);
    Overlay5Display_SetHBlankObjProcessing(engine, 0);

    for (background = 0; background < 4; background++) {
        Overlay5DisplayBg_SetOffset(engine, (u8)background, 0, 0);
    }
    for (background = 2; background < 4; background++) {
        Overlay5DisplayBg_SetAffine(engine, (u8)background, FX32_ONE, FX32_ONE, 0, 0, 0, 0, 0);
    }

    if (engine == DISPLAY_ENGINE_MAIN) {
        *(vu16 *)(REG_BG2_AFFINE + 0x0) = 0x100;
        *(vu16 *)(REG_BG2_AFFINE + 0x2) = 0;
        *(vu16 *)(REG_BG2_AFFINE + 0x4) = 0;
        *(vu16 *)(REG_BG2_AFFINE + 0x6) = 0x100;
        *(vu16 *)(REG_BG3_AFFINE + 0x0) = 0x100;
        *(vu16 *)(REG_BG3_AFFINE + 0x2) = 0;
        *(vu16 *)(REG_BG3_AFFINE + 0x4) = 0;
        *(vu16 *)(REG_BG3_AFFINE + 0x6) = 0x100;
        GX_ResetBankForBG();
        GX_ResetBankForBGExtPltt();
        GX_ResetBankForOBJ();
        GX_ResetBankForOBJExtPltt();
        GX_ResetBankForTex();
        GX_ResetBankForTexPltt();
        GX_ResetBankForClearImage();
    } else {
        *(vu16 *)(REG_BG2_AFFINE + 0x1000 + 0x0) = 0x100;
        *(vu16 *)(REG_BG2_AFFINE + 0x1000 + 0x2) = 0;
        *(vu16 *)(REG_BG2_AFFINE + 0x1000 + 0x4) = 0;
        *(vu16 *)(REG_BG2_AFFINE + 0x1000 + 0x6) = 0x100;
        *(vu16 *)(REG_BG3_AFFINE + 0x1000 + 0x0) = 0x100;
        *(vu16 *)(REG_BG3_AFFINE + 0x1000 + 0x2) = 0;
        *(vu16 *)(REG_BG3_AFFINE + 0x1000 + 0x4) = 0;
        *(vu16 *)(REG_BG3_AFFINE + 0x1000 + 0x6) = 0x100;
        GX_ResetBankForSubBG();
        GX_ResetBankForSubBGExtPltt();
        GX_ResetBankForSubOBJ();
        GX_ResetBankForSubOBJExtPltt();
    }
}

void Overlay5Display_SetVisiblePlanes(DisplayEngine engine, u32 planes) {
    vu32 *display_control = (vu32 *)(REG_DISPCNT + (engine << 12));
    *display_control = (*display_control & ~DISPCNT_VISIBLE_PLANE_MASK) | (planes << 8);
}

void Overlay5Display_EnablePlanes(DisplayEngine engine, u32 planes) {
    vu32 *display_control = (vu32 *)(REG_DISPCNT + (engine << 12));
    *display_control |= planes << 8;
}

void Overlay5Display_DisablePlanes(DisplayEngine engine, u32 planes) {
    vu32 *display_control = (vu32 *)(REG_DISPCNT + (engine << 12));
    *display_control &= ~(planes << 8);
}

void Overlay5Display_SetVisibleWindows(DisplayEngine engine, u32 windows) {
    vu32 *display_control = (vu32 *)(REG_DISPCNT + (engine << 12));
    *display_control = (*display_control & ~DISPCNT_VISIBLE_WINDOW_MASK) | (windows << 13);
}

void Overlay5Display_DisableWindows(DisplayEngine engine, u32 windows) {
    vu32 *display_control = (vu32 *)(REG_DISPCNT + (engine << 12));
    *display_control &= ~(windows << 13);
}

void Overlay5Display_SetHBlankObjProcessing(DisplayEngine engine, int enabled) {
    vu32 *display_control = (vu32 *)(REG_DISPCNT + (engine << 12));

    if (enabled) {
        *display_control |= DISPCNT_HBLANK_OBJ_PROCESSING;
    } else {
        *display_control &= ~DISPCNT_HBLANK_OBJ_PROCESSING;
    }
}

void Overlay5Display_SetObjTileMappingMode(
    DisplayEngine engine, DisplayObjTileMappingMode mode
) {
    switch (mode) {
    case DISPLAY_OBJ_TILE_MAPPING_2D:
    case DISPLAY_OBJ_TILE_MAPPING_1D_32K:
        gOverlay5DisplayObjTileBoundary[engine] = 1;
        break;
    case DISPLAY_OBJ_TILE_MAPPING_1D_64K:
        gOverlay5DisplayObjTileBoundary[engine] = 2;
        break;
    case DISPLAY_OBJ_TILE_MAPPING_1D_128K:
        gOverlay5DisplayObjTileBoundary[engine] = 4;
        break;
    case DISPLAY_OBJ_TILE_MAPPING_1D_256K:
        gOverlay5DisplayObjTileBoundary[engine] = 8;
        break;
    }

    if (engine == DISPLAY_ENGINE_MAIN) {
        *(vu32 *)REG_DISPCNT =
            (*(vu32 *)REG_DISPCNT & ~DISPCNT_OBJ_TILE_MAPPING_MASK) | mode;
    } else {
        *(vu32 *)(REG_DISPCNT + 0x1000) =
            (*(vu32 *)(REG_DISPCNT + 0x1000) & ~DISPCNT_OBJ_TILE_MAPPING_MASK) | mode;
    }
}

void Overlay5Display_LoadObjTiles(
    DisplayEngine engine, const void *source, u32 offset, u32 size
) {
    void *destination = Overlay5Display_GetObjVram(engine);
    destination = (void *)(offset + (u32)destination);
    MIi_CpuCopy16(source, destination, size);
}

void Overlay5Display_LoadObjTilesLZ(DisplayEngine engine, const void *source, u32 offset) {
    MI_UncompressLZ16(source, (void *)(offset + (u32)Overlay5Display_GetObjVram(engine)));
}

void Overlay5Display_LoadObjPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
) {
    void *destination = Overlay5Display_GetObjPalette(engine);
    destination = (void *)(offset + (u32)destination);
    MIi_CpuCopy16(source, destination, size);
}

void Overlay5Display_LoadBgPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
) {
    void *destination = Overlay5Display_GetBgPalette(engine);
    destination = (void *)(offset + (u32)destination);
    MIi_CpuCopy16(source, destination, size);
}

void Overlay5DisplayBg_LoadScreen(
    DisplayEngine engine, int background, const void *source, u32 offset, u32 size
) {
    MI_CpuCopy8(
        source,
        (void *)(offset + (u32)Overlay5DisplayBg_GetScreenVram2K(engine, background)),
        size
    );
}

void Overlay5DisplayBg_LoadScreenLZ(
    DisplayEngine engine, int background, const void *source, u32 offset
) {
    MI_UncompressLZ16(
        source, (void *)(offset + (u32)Overlay5DisplayBg_GetScreenVram2K(engine, background))
    );
}

void Overlay5DisplayBg_LoadCharacters(
    DisplayEngine engine, int background, const void *source, u32 offset, u32 size
) {
    void *destination = Overlay5DisplayBg_GetCharacterVram(engine, background);
    destination = (void *)(offset + (u32)destination);
    MIi_CpuCopy16(source, destination, size);
}

void Overlay5DisplayBg_LoadCharactersLZ(
    DisplayEngine engine, int background, const void *source, u32 offset
) {
    MI_UncompressLZ16(
        source, (void *)(offset + (u32)Overlay5DisplayBg_GetCharacterVram(engine, background))
    );
}

void Overlay5Display_BeginLoadBgExtPalette(DisplayEngine engine) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        GX_BeginLoadBGExtPltt();
    } else {
        GXS_BeginLoadBGExtPltt();
    }
}

void Overlay5Display_LoadBgExtPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        GX_LoadBGExtPltt(source, offset, size);
    } else {
        GXS_LoadBGExtPltt(source, offset, size);
    }
}

void Overlay5Display_EndLoadBgExtPalette(DisplayEngine engine) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        GX_EndLoadBGExtPltt();
    } else {
        GXS_EndLoadBGExtPltt();
    }
}

void Overlay5Display_BeginLoadObjExtPalette(DisplayEngine engine) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        GX_BeginLoadOBJExtPltt();
    } else {
        GXS_BeginLoadOBJExtPltt();
    }
}

void Overlay5Display_LoadObjExtPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        GX_LoadOBJExtPltt(source, offset, size);
    } else {
        GXS_LoadOBJExtPltt(source, offset, size);
    }
}

void Overlay5Display_EndLoadObjExtPalette(DisplayEngine engine) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        GX_EndLoadOBJExtPltt();
    } else {
        GXS_EndLoadOBJExtPltt();
    }
}

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

void Overlay5DisplayBg_SetAffineMatrix(
    DisplayEngine engine,
    int background,
    const DisplayAffineMatrix *matrix,
    int center_x,
    int center_y,
    int origin_x,
    int origin_y
) {
    if (engine == DISPLAY_ENGINE_MAIN) {
        if (background == 2) {
            G2x_SetBGyAffine_(
                REG_BG2_AFFINE, matrix, center_x, center_y, origin_x, origin_y
            );
        } else {
            G2x_SetBGyAffine_(
                REG_BG3_AFFINE, matrix, center_x, center_y, origin_x, origin_y
            );
        }
    } else if (background == 2) {
        G2x_SetBGyAffine_(
            REG_BG2_AFFINE + 0x1000, matrix, center_x, center_y, origin_x, origin_y
        );
    } else {
        G2x_SetBGyAffine_(
            REG_BG3_AFFINE + 0x1000, matrix, center_x, center_y, origin_x, origin_y
        );
    }
}

void Overlay5DisplayBg_SetAffine(
    DisplayEngine engine,
    int background,
    fx32 horizontal_scale,
    fx32 vertical_scale,
    int rotation,
    int center_x,
    int center_y,
    int origin_x,
    int origin_y
) {
    s16 sine = FX_SinCosTable_[((rotation & 0xFFFF) >> 4) * 2];
    s16 cosine = FX_SinCosTable_[((rotation & 0xFFFF) >> 4) * 2 + 1];
    DisplayAffineMatrix matrix;

    if (horizontal_scale == 0) {
        horizontal_scale = 0x100000;
    } else {
        horizontal_scale = 0x100000 / horizontal_scale;
    }
    if (vertical_scale == 0) {
        vertical_scale = 0x100000;
    } else {
        vertical_scale = 0x100000 / vertical_scale;
    }

    matrix.m00 = (horizontal_scale * cosine) >> 8;
    matrix.m01 = (horizontal_scale * sine) >> 8;
    matrix.m10 = -((vertical_scale * sine) >> 8);
    matrix.m11 = (vertical_scale * cosine) >> 8;
    Overlay5DisplayBg_SetAffineMatrix(
        engine, background, &matrix, center_x, center_y, origin_x, origin_y
    );
}
