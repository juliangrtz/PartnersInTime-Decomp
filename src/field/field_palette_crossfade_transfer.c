#include <game/field_palette_crossfade.h>
extern u8 *data_ov000_020bfc5c[14];
extern u32 func_020357b4(void), func_0203582c(void), func_020357dc(void), func_02035850(void);
extern void func_020358ac(u32), func_02035e04(u32), func_02035938(u32), func_02035ebc(u32);
extern void func_0202cc58(const void *, void *, u32), func_0202cd2c(const void *, void *, u32);
static inline void CopyPalette(const void *source, void *dest, int size)
{
    if ((u32)source < (u32)dest)
        func_0202cd2c(source, dest, size);
    else
        func_0202cc58(source, dest, size);
}
void FieldPaletteCrossfade_Transfer(FieldAreaContext *field)
{
    u8 *target;
    int size;
    int main_obj, main_bg, sub_obj, sub_bg;
    u8 *base, *source;
    if (!field->palette_crossfade || !field->palette_crossfade->flags.active)
        return;
    /* Extended palettes must temporarily be accessible through the LCDC mapping.
     * Preserve each engine's bank assignment for restoration after the copies. */
    switch (field->palette_crossfade->flags.region) {
    case FIELD_PALETTE_OBJ_EXTENDED:
        if (!field->flags.screen)
            main_obj = func_0203582c();
        else
            sub_obj = func_020357b4();
        size = 512;
        break;
    case FIELD_PALETTE_BG_EXTENDED_0:
    case FIELD_PALETTE_BG_EXTENDED_1:
    case FIELD_PALETTE_BG_EXTENDED_2:
    case FIELD_PALETTE_BG_EXTENDED_3:
        if (!field->flags.screen)
            main_bg = func_02035850();
        else
            sub_bg = func_020357dc();
        size = 512;
        break;
    default:
        size = 32;
        break;
    }
    base = data_ov000_020bfc5c[7 * field->flags.screen + field->palette_crossfade->flags.region];
    source = base + field->palette_crossfade->flags.source_bank * size;
    target = base + field->palette_crossfade->flags.target_bank * size;
    /* Capture both endpoints once; subsequent frames replace the source bank
     * with the interpolated colors prepared by the update routine. */
    if (!field->palette_crossfade->flags.captured) {
        CopyPalette(source, field->palette_crossfade->source, size);
        CopyPalette(target, field->palette_crossfade->target, size);
        field->palette_crossfade->flags.captured = 1;
    } else
        CopyPalette(field->palette_crossfade->colors, source, size);
    switch (field->palette_crossfade->flags.region) {
    case FIELD_PALETTE_OBJ_EXTENDED:
        if (!field->flags.screen)
            func_02035e04(main_obj);
        else
            func_020358ac(sub_obj);
        break;
    case FIELD_PALETTE_BG_EXTENDED_0:
    case FIELD_PALETTE_BG_EXTENDED_1:
    case FIELD_PALETTE_BG_EXTENDED_2:
    case FIELD_PALETTE_BG_EXTENDED_3:
        if (!field->flags.screen)
            func_02035ebc(main_bg);
        else
            func_02035938(sub_bg);
        break;
    }
    if (field->palette_crossfade->time == field->palette_crossfade->duration)
        field->palette_crossfade->flags.active = 0;
}
