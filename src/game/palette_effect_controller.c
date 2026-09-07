#include <game/palette_animation.h>
#include <game/sprite_output.h>
#include <game/heap.h>

extern const void *data_02050b78[];

GamePaletteEffectController *GamePaletteEffects_Init(GamePaletteEffectController *controller,
                                          u8 screen, GamePaletteEffectEntry *entries, int count)
{
    int index;
    controller->vtable = data_02050b78;
    controller->screen = screen;
    controller->count = count;
    controller->entries = entries;
    for (index = 0; index < count; ++index) GamePaletteEffects_ResetEntry(controller, (u8)index);
    return controller;
}

GamePaletteEffectController *GamePaletteEffects_Destroy(GamePaletteEffectController *controller)
{
    controller->vtable = data_02050b78;
    return controller;
}

GamePaletteEffectController *GamePaletteEffects_Delete(GamePaletteEffectController *controller)
{
    controller->vtable = data_02050b78;
    GameHeap_Delete(controller);
    return controller;
}
