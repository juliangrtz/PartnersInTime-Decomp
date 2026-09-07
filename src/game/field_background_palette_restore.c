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
