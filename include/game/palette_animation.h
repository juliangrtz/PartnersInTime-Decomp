#ifndef PIT_GAME_PALETTE_ANIMATION_H
#define PIT_GAME_PALETTE_ANIMATION_H
#include <nitro.h>

typedef struct GamePaletteEffectEntry {
    s16 mode;
    u16 color;
    u32 palette_mask;
    s32 duration, elapsed_q8;
    s16 speed_q8;
    u8 repeat_mode;
    union { u8 raw; struct { u8 paused:1, ended:1, unknown:6; } bits; } flags;
} GamePaletteEffectEntry;
typedef struct GamePaletteEffectController {
    const void *vtable;
    u8 screen, count, unknown_06[2];
    GamePaletteEffectEntry *entries;
} GamePaletteEffectController;
typedef char GamePaletteEffectEntry_SizeCheck[sizeof(GamePaletteEffectEntry) == 20 ? 1 : -1];
typedef char GamePaletteEffectController_SizeCheck[sizeof(GamePaletteEffectController) == 12 ? 1 : -1];

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
GamePaletteEffectController *GamePaletteEffects_Init(GamePaletteEffectController *controller,
                                          u8 screen, GamePaletteEffectEntry *entries, int count);
GamePaletteEffectController *GamePaletteEffects_Destroy(GamePaletteEffectController *controller);
GamePaletteEffectController *GamePaletteEffects_Delete(GamePaletteEffectController *controller);
void GamePaletteEffects_ConfigureEntry(GamePaletteEffectController *controller, int index, u32 mask,
                   s16 mode, u16 color, int duration, u8 repeat_mode, s16 speed_q8,
                   int elapsed_q8, u8 paused);
void GamePaletteEffects_ResetEntry(GamePaletteEffectController *controller, int index);
void GamePaletteEffects_Reverse(GamePaletteEffectController *controller, int index);
int GamePaletteEffects_IsPaused(GamePaletteEffectController *controller, int index);
GamePaletteEffectEntry *GamePaletteEffects_GetEntry(GamePaletteEffectController *controller, int index);

#ifdef __cplusplus
}
#endif
#endif
