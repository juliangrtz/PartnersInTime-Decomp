#include <game/sprite_effect.h>

extern s32 FX_Div(s32 numerator, s32 denominator);

void GameSpriteEffect_ScaleSize(s32 scale_x, s32 scale_y, GameSpriteEffectSize *size)
{
    if (scale_x != 4096) {
        s32 width = FX_Div((size->bytes.width + 1) << 12, scale_x);
        if (scale_x < 4096) width += scale_x;
        width = (width >> 12) - 1;
        if (width < 0) width = 0;
        size->bytes.width = width;
    }
    if (scale_y != 4096) {
        s32 height = FX_Div((size->bytes.height + 1) << 12, scale_y);
        if (scale_y < 4096) height += scale_y;
        height = (height >> 12) - 1;
        if (height < 0) height = 0;
        size->bytes.height = height;
    }
}
