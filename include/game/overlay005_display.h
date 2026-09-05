#ifndef GAME_OVERLAY005_DISPLAY_H
#define GAME_OVERLAY005_DISPLAY_H

#include <nitro.h>
#include <nitro/fx.h>

typedef enum DisplayEngine {
    DISPLAY_ENGINE_MAIN = 0,
    DISPLAY_ENGINE_SUB = 1,
} DisplayEngine;

typedef enum DisplayObjTileMappingMode {
    DISPLAY_OBJ_TILE_MAPPING_2D = 0x000000,
    DISPLAY_OBJ_TILE_MAPPING_1D_32K = 0x000010,
    DISPLAY_OBJ_TILE_MAPPING_1D_64K = 0x100010,
    DISPLAY_OBJ_TILE_MAPPING_1D_128K = 0x200010,
    DISPLAY_OBJ_TILE_MAPPING_1D_256K = 0x300010,
} DisplayObjTileMappingMode;

typedef enum DisplayPlaneMask {
    DISPLAY_PLANE_NONE = 0,
    DISPLAY_PLANE_BG0 = 1 << 0,
    DISPLAY_PLANE_BG1 = 1 << 1,
    DISPLAY_PLANE_BG2 = 1 << 2,
    DISPLAY_PLANE_BG3 = 1 << 3,
    DISPLAY_PLANE_OBJ = 1 << 4,
} DisplayPlaneMask;

typedef enum DisplayWindowMask {
    DISPLAY_WINDOW_NONE = 0,
    DISPLAY_WINDOW_0 = 1 << 0,
    DISPLAY_WINDOW_1 = 1 << 1,
    DISPLAY_WINDOW_OBJ = 1 << 2,
} DisplayWindowMask;

typedef struct DisplayAffineMatrix {
    fx32 m00;
    fx32 m01;
    fx32 m10;
    fx32 m11;
} DisplayAffineMatrix;

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
);
void Overlay5DisplayBg_SetAffineMatrix(
    DisplayEngine engine,
    int background,
    const DisplayAffineMatrix *matrix,
    int center_x,
    int center_y,
    int origin_x,
    int origin_y
);

void Overlay5Display_SetObjTileMappingMode(
    DisplayEngine engine, DisplayObjTileMappingMode mode
);
void Overlay5Display_SetHBlankObjProcessing(DisplayEngine engine, int enabled);
void Overlay5Display_DisableWindows(DisplayEngine engine, u32 windows);
void Overlay5Display_SetVisibleWindows(DisplayEngine engine, u32 windows);
void Overlay5Display_DisablePlanes(DisplayEngine engine, u32 planes);
void Overlay5Display_EnablePlanes(DisplayEngine engine, u32 planes);
void Overlay5Display_SetVisiblePlanes(DisplayEngine engine, u32 planes);
void Overlay5Display_ResetEngine(DisplayEngine engine);

void Overlay5Display_EndLoadObjExtPalette(DisplayEngine engine);
void Overlay5Display_LoadObjExtPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
);
void Overlay5Display_BeginLoadObjExtPalette(DisplayEngine engine);
void Overlay5Display_EndLoadBgExtPalette(DisplayEngine engine);
void Overlay5Display_LoadBgExtPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
);
void Overlay5Display_BeginLoadBgExtPalette(DisplayEngine engine);
void Overlay5DisplayBg_LoadCharactersLZ(
    DisplayEngine engine, int background, const void *source, u32 offset
);
void Overlay5DisplayBg_LoadCharacters(
    DisplayEngine engine, int background, const void *source, u32 offset, u32 size
);
void Overlay5DisplayBg_LoadScreenLZ(
    DisplayEngine engine, int background, const void *source, u32 offset
);
void Overlay5DisplayBg_LoadScreen(
    DisplayEngine engine, int background, const void *source, u32 offset, u32 size
);
void Overlay5Display_LoadBgPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
);
void Overlay5Display_LoadObjPalette(
    DisplayEngine engine, const void *source, u32 offset, u32 size
);
void Overlay5Display_LoadObjTilesLZ(DisplayEngine engine, const void *source, u32 offset);
void Overlay5Display_LoadObjTiles(
    DisplayEngine engine, const void *source, u32 offset, u32 size
);

void Overlay5DisplayBg_SetVerticalOffset(DisplayEngine engine, int background, s16 y);
void Overlay5DisplayBg_SetOffset(DisplayEngine engine, int background, s16 x, s16 y);
void Overlay5DisplayBg_SetOverflow(DisplayEngine engine, int background, u16 enabled);
void Overlay5DisplayBg_SetAreaOverflow(DisplayEngine engine, int background, u16 enabled);
void Overlay5DisplayBg_SetCharacterBase(DisplayEngine engine, int background, u16 block);
void Overlay5DisplayBg_SetScreenBase(DisplayEngine engine, int background, u16 block);
void Overlay5DisplayBg_SetModeBits(DisplayEngine engine, int background, u16 mode_bits);
void Overlay5DisplayBg_SetColorMode(DisplayEngine engine, int background, u16 mode);
void Overlay5DisplayBg_SetScreenSize(DisplayEngine engine, int background, u16 size);
void Overlay5DisplayBg_SetMosaic(DisplayEngine engine, int background, int enabled);
void Overlay5DisplayBg_SetPriority(DisplayEngine engine, int background, int priority);
void Overlay5DisplayBg_ClearControl(DisplayEngine engine, int background);

vu16 *Overlay5DisplayBg_GetControl(DisplayEngine engine, int background);
void *Overlay5Display_GetObjPalette(DisplayEngine engine);
void *Overlay5Display_GetBgPalette(DisplayEngine engine);
void *Overlay5Display_GetObjVram(DisplayEngine engine);
void *Overlay5DisplayBg_GetCharacterVram(DisplayEngine engine, int background);
void *Overlay5DisplayBg_GetScreenVram16K(DisplayEngine engine, int background);
void *Overlay5DisplayBg_GetScreenVram2K(DisplayEngine engine, int background);

#endif
