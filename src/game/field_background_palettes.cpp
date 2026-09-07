#include <game/field_background.h>
#include <game/palette_animation.h>
extern "C" {
#include <game/heap.h>
extern void func_0202cbd4(void *, int, u32);
extern void func_0202cc58(const void *, void *, u32);
extern void func_0202cd2c(const void *, void *, u32);
extern void func_0203b7b4(const void *, void *, u32);
extern void DC_FlushRange(const void *, u32);
extern void GXS_BeginLoadBGExtPltt(void), GX_BeginLoadBGExtPltt(void);
extern void GXS_LoadBGExtPltt(const void *, u32, u32), GX_LoadBGExtPltt(const void *, u32, u32);
extern void GXS_EndLoadBGExtPltt(void), GX_EndLoadBGExtPltt(void);
extern void func_02038640(const void *, u32, u32), func_020385d8(const void *, u32, u32);

extern void MI_CpuFill8(void *, u8, u32);
extern int func_0202dca8(const s16 *, int, u16 *, int, const u16 *, int);

static inline void CopyPalette(const void *source, void *destination, u32 size)
{
    if (source < destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
}

int FieldBackground_StopTileAnimation(FieldBackground *background, int index)
{
    u8 *active = &background->tile_animations[index].active;
    if (!*active) return 0;
    *active = 0;
    return 1;
}

int FieldBackground_StartTileAnimation(FieldBackground *background, int index)
{
    u8 *active = &background->tile_animations[index].active;
    if (*active == 1) return 0;
    *active = 1;
    return 1;
}

const s16 *FieldBackground_GetPaletteAnimation(FieldBackground *background, u8 *state,
                        u16 **first, u16 **second, u16 **third)
{
    *state = background->palette_state.bits.animation;
    *first = !(background->color256_layers & 1) ? background->palette : background->extended_palettes[0];
    *second = !(background->color256_layers & 2) ? background->palette : background->extended_palettes[1];
    *third = !(background->color256_layers & 4) ? background->palette : background->extended_palettes[2];
    return background->palette_animation_table;
}

void FieldBackground_SetPaletteAnimation(FieldBackground *background, const s16 *table)
{
    background->palette_animation_table = table;
}

void FieldBackground_InitializePaletteAnimation(FieldBackground *background)
{
    const s16 *table = background->palette_animation_table;
    background->palette_animation_count = table[1] - table[0];
    background->palette_animation_times = (s32 *)GameHeap_NewArray(
        4 * (background->palette_animation_count + 1), background->heap, 0, 0);
    func_0202cbd4(background->palette_animation_times, 0, 4 * (background->palette_animation_count + 1));
    background->palette_animation_states = (u8 *)GameHeap_NewArray(
        background->palette_animation_count + 1, background->heap, 0, 0);
    MI_CpuFill8(background->palette_animation_states, 0, background->palette_animation_count + 1);
    background->palette_components[0] = GameAnimationTable_GetComponent(table, 1, -1);
    background->palette_components[1] = GameAnimationTable_GetComponent(table, 2, -1);
    background->palette_components[2] = GameAnimationTable_GetComponent(table, 3, -1);
    if (background->palette_components[0] || background->palette_components[1] || background->palette_components[2]) {
        background->palette_state.bits.animation = 1;
        background->palette_animation_states[0] = 0x11;
    }
}

int FieldBackground_HasPaletteAnimation(FieldBackground *background)
{
    int index = 0;
    u8 *state;
    int count = background->palette_animation_count + 1;
    if (count > 0) {
        state = background->palette_animation_states;
        do {
            if (*state & 0xf) return 1;
            ++index;
            ++state;
        } while (index < count);
    }
    return 0;
}

void FieldBackground_StartPaletteAnimation(FieldBackground *background, int index, u8 flags)
{
    background->palette_animation_states[index + 1] = flags | 1;
    background->palette_animation_times[index + 1] = 0;
    background->palette_state.bits.animation = 1;
}

void FieldBackground_ClearPaletteAnimation(FieldBackground *background, int index)
{
    background->palette_animation_states[index + 1] = 0;
    background->palette_animation_times[index + 1] = 0;
    if (!background->has_palette_animation())
        background->palette_state.bits.animation = 0;
}

void FieldBackground_ClearAllPaletteAnimations(FieldBackground *background)
{
    int i;
    for (i = 0; i < background->palette_animation_count + 1; ++i) {
        background->palette_animation_states[i] = 0;
        background->palette_animation_times[i] = 0;
    }
    background->palette_state.bits.animation = 0;
    background->restore_and_upload_palette();
}

void FieldBackground_PausePaletteAnimation(FieldBackground *background, int index)
{
    background->palette_animation_states[index + 1] &= 0xf0;
    if (!background->has_palette_animation())
        background->palette_state.bits.animation = 0;
}

void FieldBackground_ResumePaletteAnimation(FieldBackground *background, int index)
{
    background->palette_animation_states[index + 1] =
        (background->palette_animation_states[index + 1] & 0xf0) | 1;
    if (background->palette_state.bits.animation == 0)
        background->palette_state.bits.animation = 1;
}

void FieldBackground_RestoreAndUploadPalette(FieldBackground *background)
{
    background->restore_palettes();
    background->transfers->dirty_flags |= 2;
}

void FieldBackground_RestorePalettes(FieldBackground *background)
{
    int i;
    int standard_layer = -1;
    for (i = 0; i < 3; ++i) {
        if (background->palette_sources[i].size) {
            if (background->color256_layers & (1 << i)) {
                CopyPalette(background->palette_sources[i].colors,
                            background->extended_palettes[i], background->palette_sources[i].size);
            } else {
                standard_layer = i;
            }
        }
    }
    if (standard_layer != -1)
        CopyPalette(background->base_palette, background->palette, 512);
}

void FieldBackground_UpdatePaletteAnimations(FieldBackground *background)
{
    int layer;
    int index;
    /* The original keeps this maximum across all active tracks. */
    int maximum = -1;
    background->restore_palettes();
    for (index = 0; index < background->palette_animation_count + 1; ++index) {
        if (background->palette_animation_states[index] & 0xf) {
            background->palette_components[0] = GameAnimationTable_GetComponent(background->palette_animation_table, 1, index - 1);
            background->palette_components[1] = GameAnimationTable_GetComponent(background->palette_animation_table, 2, index - 1);
            background->palette_components[2] = GameAnimationTable_GetComponent(background->palette_animation_table, 3, index - 1);
            background->palette_animation_times[index] += background->palette_animation_speed;
            for (layer = 0; layer < 3; ++layer) {
                if (background->palette_sources[layer].size) {
                    u16 *palette;
                    int width;
                    if (background->color256_layers & (1 << layer)) {
                        palette = background->extended_palettes[layer];
                        width = 256;
                    } else {
                        palette = background->palette;
                        width = 16;
                    }
                    int duration = func_0202dca8(background->palette_components[layer],
                        background->palette_animation_times[index], palette, width, 0, 1);
                    if (maximum < duration) maximum = duration;
                }
            }
            if ((maximum << 8) <= background->palette_animation_times[index]) {
                if (!(background->palette_animation_states[index] & 0xf0))
                    background->palette_animation_states[index] = 0;
                background->palette_animation_times[index] = 0;
            }
        }
    }
    if (background->has_palette_animation())
        background->palette_state.bits.animation = 1;
    else
        background->palette_state.bits.animation = 0;
}

void FieldBackground_UploadAllPalettes(FieldBackground *background)
{
    if (background->extended_palettes[0])
        DC_FlushRange(background->extended_palettes[0], background->palette_sources[0].size);
    if (background->extended_palettes[1])
        DC_FlushRange(background->extended_palettes[1], background->palette_sources[1].size);
    if (background->extended_palettes[2])
        DC_FlushRange(background->extended_palettes[2], background->palette_sources[2].size);
    DC_FlushRange(background->palette, 512);
    if (!background->screen) {
        GX_BeginLoadBGExtPltt();
        if ((background->color256_layers & 1) && background->palette_sources[0].size)
            GX_LoadBGExtPltt(background->extended_palettes[0], 0x2000, background->palette_sources[0].size);
        if ((background->color256_layers & 2) && background->palette_sources[1].size)
            GX_LoadBGExtPltt(background->extended_palettes[1], 0x4000, background->palette_sources[1].size);
        if ((background->color256_layers & 4) && background->palette_sources[2].size)
            GX_LoadBGExtPltt(background->extended_palettes[2], 0x6000, background->palette_sources[2].size);
        GX_EndLoadBGExtPltt();
        if (!(background->color256_layers & 1) || !(background->color256_layers & 2)
            || !(background->color256_layers & 4) || background->palette_state.bits.upload_full_palette == 1) {
            func_02038640(background->palette, 0, 512);
        } else {
            *(vu16 *)0x05000000 = background->palette_sources[2].colors[0];
            background->base_palette[0] = background->palette_sources[2].colors[0];
        }
    } else {
        GXS_BeginLoadBGExtPltt();
        if ((background->color256_layers & 1) && background->palette_sources[0].size)
            GXS_LoadBGExtPltt(background->extended_palettes[0], 0x2000, background->palette_sources[0].size);
        if ((background->color256_layers & 2) && background->palette_sources[1].size)
            GXS_LoadBGExtPltt(background->extended_palettes[1], 0x4000, background->palette_sources[1].size);
        if ((background->color256_layers & 4) && background->palette_sources[2].size)
            GXS_LoadBGExtPltt(background->extended_palettes[2], 0x6000, background->palette_sources[2].size);
        GXS_EndLoadBGExtPltt();
        if (!(background->color256_layers & 1) || !(background->color256_layers & 2)
            || !(background->color256_layers & 4) || background->palette_state.bits.upload_full_palette == 1) {
            func_020385d8(background->palette, 0, 512);
        } else {
            *(vu16 *)0x05000400 = background->palette_sources[2].colors[0];
            background->base_palette[0] = background->palette_sources[2].colors[0];
        }
    }
}

void FieldBackground_AllocatePalettes(FieldBackground *background)
{
    int i;
    int standard_layer = -1;
    background->base_palette = (u16 *)GameHeap_NewArray(512, background->heap, 0, 0);
    func_0202cbd4(background->base_palette, 0, 512);
    for (i = 0; i < 3; ++i) {
        u32 size = background->palette_sources[i].size;
        if (size) {
            if (background->color256_layers & (1 << i)) {
                background->extended_palettes[i] = (u16 *)GameHeap_NewArray(2 * (size >> 1), background->heap, 0, 0);
                func_0203b7b4(background->palette_sources[i].colors, background->extended_palettes[i],
                              background->palette_sources[i].size);
            } else {
                standard_layer = (s8)i;
            }
        }
    }
    background->palette = (u16 *)GameHeap_NewArray(512, background->heap, 0, 0);
    if (standard_layer != -1) {
        CopyPalette(background->palette_sources[standard_layer].colors, background->palette, 512);
        CopyPalette(background->palette_sources[standard_layer].colors, background->base_palette, 512);
    }
}

void FieldBackground_RequestPaletteUpload(FieldBackground *background)
{
    background->upload_all_palettes();
}
}
