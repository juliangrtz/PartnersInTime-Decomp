#include <game/field_background.h>
extern "C" {
extern void func_0203b7b4(const void *, void *, u32);

typedef struct FieldLayerParameterTable { s32 values[16]; } FieldLayerParameterTable;
extern const FieldLayerParameterTable data_02050d18;

void FieldBackground_UpdateBlendAnimation(FieldBackground *background)
{
    if (background->blend_animation) {
        background->blend_time_q8 -= background->palette_animation_speed;
        if (background->blend_time_q8 <= 0) {
            const FieldBackgroundBlendFrame *frames = background->blend_animation->frames;
            ++background->blend_frame;
            if (background->blend_animation->count <= background->blend_frame) {
                background->blend_frame = 0;
                if (background->blend_animation->stop_at_end == 1) {
                    background->blend_animation = 0;
                    return;
                }
            }
            background->transfers->queue_blend(background->blend_animation->planes_a * 2,
                background->blend_animation->planes_b * 2,
                frames[background->blend_frame].coefficient_a,
                frames[background->blend_frame].coefficient_b);
            background->blend_time_q8 += frames[background->blend_frame].duration << 8;
        }
    }
}

void *FieldBackground_GetCharacterBase(FieldBackground *background, int block)
{
    if (background->screen)
        return (void *)(0x06200000 + (block << 14));
    return (void *)(0x06000000 + (block << 14));
}

void FieldBackground_GetLayerParameters(FieldBackground *background, int layer, int *x, int *y)
{
    FieldLayerParameterTable parameters = data_02050d18;
    switch (layer) {
    case 0:
        *x = parameters.values[background->configuration->first_x];
        *y = parameters.values[background->configuration->first_y];
        break;
    case 1:
        *x = parameters.values[background->configuration->second_x];
        *y = parameters.values[background->configuration->second_y];
        break;
    case 2:
        *x = parameters.values[background->configuration->third_x];
        *y = parameters.values[background->configuration->third_y];
        break;
    }
}

int FieldBackground_GetPaletteSpan(FieldBackground *background, u32 mask)
{
    int i;
    for (i = 0; i < 6; ++i) {
        if (mask & 1) break;
        mask >>= 1;
    }
    return mask ? mask << 9 : 512;
}

void FieldBackground_SetPaletteEffects(FieldBackground *background, FieldBackgroundPaletteEffect *effects,
                   u8 count, int reset)
{
    background->palette_effects = effects;
    background->palette_effect_count = count;
    if (reset) {
        for (int i = 0; i < background->palette_effect_count; ++i)
            background->reset_palette_effect((u8)i);
    }
}

void FieldBackground_ConfigurePaletteEffect(FieldBackground *background, int index,
    u16 first_mask, u16 second_mask, u16 third_mask, u16 standard_mask,
    s16 mode, u16 color, int duration, u8 finish_mode, s16 speed, int time, u8 disabled)
{
    FieldBackgroundPaletteEffect *effect = &background->palette_effects[index];
    effect->mode = mode;
    effect->color = color;
    effect->extended_masks[0] = first_mask;
    effect->extended_masks[1] = second_mask;
    effect->extended_masks[2] = third_mask;
    effect->standard_mask = standard_mask;
    effect->duration = duration;
    effect->time_q8 = time;
    effect->speed_q8 = speed;
    effect->finish_mode = finish_mode;
    effect->disabled = disabled;
    if (background->palette_state.bits.animation == 2)
        background->palette_state.bits.animation = 3;
}

void FieldBackground_ResetPaletteEffect(FieldBackground *background, int index)
{
    FieldBackgroundPaletteEffect *effect = &background->palette_effects[index];
    effect->mode = 0;
    effect->color = 0;
    effect->extended_masks[0] = 0;
    effect->extended_masks[1] = 0;
    effect->extended_masks[2] = 0;
    effect->standard_mask = 0;
    effect->duration = 0;
    effect->time_q8 = 0;
    effect->speed_q8 = 256;
    effect->finish_mode = 0;
    effect->disabled = 1;
}

u16 *FieldBackground_EditBasePalette(FieldBackground *background)
{
    background->palette_state.raw |= 0x10;
    return background->base_palette;
}

void FieldBackground_UploadBasePalette(FieldBackground *background)
{
    func_0203b7b4(background->base_palette, background->palette, 512);
    background->transfers->dirty_flags |= 2;
}
}
