#include <game/field_background.h>

void FieldBackground_ReversePaletteEffect(FieldBackground *background, int index)
{
    FieldBackgroundPaletteEffect *effect = &background->palette_effects[index];
    effect->time_q8 = (effect->duration << 8) - effect->time_q8;
    effect->mode = -effect->mode;
}
