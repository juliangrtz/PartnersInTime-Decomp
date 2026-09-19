/*
 * Field effect controls (overlay 0, 0x020756AC-0x02075814).
 *
 * Whether the area's palette and background effects are running, and pausing or
 * reversing them.
 */

#include <game/field_area.h>
#include <game/field_background.h>
#include <game/palette_animation.h>
typedef struct FieldEffectView {
    u8 unknown0000[9472];
    FieldBackground *background;
    FieldBackgroundPaletteEffect background_entries[2];
    GamePaletteEffectController *palettes;
    GamePaletteEffectEntry palette_entries[2];
} FieldEffectView;
typedef char FieldEffectViewSize[sizeof(FieldEffectView) == 9568 ? 1 : -1];
extern "C" int FieldArea_AreEffectsActive(FieldAreaContext *area, int index) {
    FieldEffectView *effects = (FieldEffectView *)area;
    return (effects->palettes && effects->palette_entries[index].mode &&
            !effects->palette_entries[index].flags.bits.paused) ||
           (effects->background && effects->background_entries[index].mode &&
            !effects->background_entries[index].disabled);
}

extern "C" void FieldArea_SetEffectsPaused(FieldAreaContext *area, int index, u8 paused) {
    FieldEffectView *effects = (FieldEffectView *)area;
    if (effects->palettes)
        effects->palette_entries[index].flags.bits.paused = paused;
    if (effects->background)
        effects->background_entries[index].disabled = paused;
}

extern "C" void FieldArea_ReverseEffects(FieldAreaContext *area, int index) {
    FieldEffectView *effects = (FieldEffectView *)area;
    FieldArea_SetEffectsPaused(area, index, 0);
    if (effects->palettes)
        GamePaletteEffects_Reverse(effects->palettes, (u8)index);
    if (effects->background) {
        effects->background->reverse_palette_effect((u8)index);
        effects->background_entries[index].finish_mode =
            (s8)effects->background_entries[index].finish_mode ^ 1;
    }
}
