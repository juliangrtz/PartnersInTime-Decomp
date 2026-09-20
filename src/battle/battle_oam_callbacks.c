/*
 * Battle numeric and cached-sprite draw callbacks (overlay 2,
 * 0x0206A674-0x0206A7A4). The callback renderer owns a 28-byte numeric payload
 * or a 24-byte cached OAM payload. Source widths other than 2 and 4 use a byte.
 */
#include "battle_oam_internal.h"
void BattleCachedOamModel_Draw(BattleModel *model, void *output, u8 *object_count, u8 *affine_count)
{
    BattleOamDisplay *display = ((BattleCallbackModelView *)model)->owned_buffer;
    if ((u8)model->flag_bits.animation_active) {
        func_ov002_0206acbc(output, object_count, affine_count, display->objects,
                            (u8)display->count, model->animation_offset_x,
                            model->animation_offset_y, &display->transform, 1);
    }
}
void BattleNumberModel_Draw(BattleModel *model, void *output, u8 *object_count, u8 *affine_count)
{
    BattleNumberDisplay *display = ((BattleCallbackModelView *)model)->owned_buffer;
    if ((u8)model->flag_bits.animation_active) {
        const void *source = display->source;
        if (source) {
            switch (display->source_width) {
            case 1:
            default:
                display->value = *(const u8 *)source;
                break;
            case 2:
                display->value = *(const u16 *)source;
                break;
            case 4:
                display->value = *(const u32 *)source;
                break;
            }
        }
        BattleOam_DrawNumber(output, object_count, affine_count, model->animation_offset_x,
                             model->animation_offset_y, (u16)display->value, display->leading,
                             &display->transform);
    }
}
