#ifndef GAME_OVERLAY005_DISPLAY_H
#define GAME_OVERLAY005_DISPLAY_H

#include <nitro.h>

typedef enum DisplayEngine {
    DISPLAY_ENGINE_MAIN = 0,
    DISPLAY_ENGINE_SUB = 1,
} DisplayEngine;

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
