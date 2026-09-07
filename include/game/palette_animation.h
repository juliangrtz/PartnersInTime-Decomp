#ifndef PIT_GAME_PALETTE_ANIMATION_H
#define PIT_GAME_PALETTE_ANIMATION_H
#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif
int GamePaletteAnimation_ApplyEffect(int mode, int color, int duration, int time,
    u16 *destination, int count, const u16 *source, int loop);

/* RGB555 operations use an interpolation amount on the 0..32 scale. */
u32 GameColor_Blend(u32 color, u32 tint, int amount);
u32 GameColor_AddScaled(u32 color, u32 tint, int amount);
u32 GameColor_SubtractScaled(u32 color, u32 tint, int amount);
u32 GameColor_Tint(u32 color, u32 tint, int amount);
u32 GameColor_Invert(u32 color, int amount);

const s16 *GamePaletteAnimation_SelectKeyframes(const s16 *command,
    int *time, int *duration, int *from, int *to);
const s16 *GamePaletteAnimation_Invert(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Tint(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Subtract(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Add(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Blend(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Crossfade(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Reflect(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);
const s16 *GamePaletteAnimation_Cycle(const s16 *command, int time,
    u16 *destination, int count, const u16 *palette);

const s16 *GameAnimationTable_GetDefaultTrack(const s16 *table, int animation);
const s16 *GameAnimationTable_GetTrack(const s16 *table, int animation);
const s16 *GameAnimationTable_GetComponent(const s16 *table, int component, int animation);
#ifdef __cplusplus
}
#endif
#endif
