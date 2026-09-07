#include <game/palette_animation.h>
#include <game/sprite_output.h>
#include <game/heap.h>

extern const GamePaletteEffectEntry data_020499a8;

void GamePaletteEffects_ConfigureEntry(GamePaletteEffectController *controller, int index, u32 mask,
                   s16 mode, u16 color, int duration, u8 repeat_mode, s16 speed_q8,
                   int elapsed_q8, u8 paused)
{
    GamePaletteEffectEntry *entry = &controller->entries[index];
    entry->mode = mode;
    entry->color = color;
    entry->palette_mask = mask;
    entry->duration = duration;
    entry->elapsed_q8 = elapsed_q8;
    entry->speed_q8 = speed_q8;
    entry->repeat_mode = repeat_mode;
    entry->flags.bits.paused = paused;
}

void GamePaletteEffects_ResetEntry(GamePaletteEffectController *controller, int index)
{
    GamePaletteEffectEntry *entry = &controller->entries[index];
    if ((const void *)&data_020499a8 < (void *)entry)
        func_0202cd2c(&data_020499a8, entry, sizeof(*entry));
    else func_0202cc58(&data_020499a8, entry, sizeof(*entry));
}

void GamePaletteEffects_Reverse(GamePaletteEffectController *controller, int index)
{
    GamePaletteEffectEntry *entry = &controller->entries[index];
    entry->elapsed_q8 = (entry->duration << 8) - entry->elapsed_q8;
    entry->mode = -entry->mode;
}

int GamePaletteEffects_IsPaused(GamePaletteEffectController *controller, int index)
{
    return controller->entries[index].flags.bits.paused;
}

GamePaletteEffectEntry *GamePaletteEffects_GetEntry(GamePaletteEffectController *controller, int index)
{
    return &controller->entries[index];
}
