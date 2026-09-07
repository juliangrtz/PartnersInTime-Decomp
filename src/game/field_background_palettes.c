#include <game/field_background.h>
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

static inline void CopyPalette(const void *source, void *destination, u32 size)
{
    if (source < destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
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
    background->base_palette = GameHeap_NewArray(512, background->heap, 0, 0);
    func_0202cbd4(background->base_palette, 0, 512);
    for (i = 0; i < 3; ++i) {
        u32 size = background->palette_sources[i].size;
        if (size) {
            if (background->color256_layers & (1 << i)) {
                background->extended_palettes[i] = GameHeap_NewArray(2 * (size >> 1), background->heap, 0, 0);
                func_0203b7b4(background->palette_sources[i].colors, background->extended_palettes[i],
                              background->palette_sources[i].size);
            } else {
                standard_layer = (s8)i;
            }
        }
    }
    background->palette = GameHeap_NewArray(512, background->heap, 0, 0);
    if (standard_layer != -1) {
        CopyPalette(background->palette_sources[standard_layer].colors, background->palette, 512);
        CopyPalette(background->palette_sources[standard_layer].colors, background->base_palette, 512);
    }
}
