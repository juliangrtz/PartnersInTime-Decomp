/* Bro Flower palette pulse, overlay 14, 0x020C2BB4-0x020C2D24. */
#include "flower_internal.h"
#include <game/palette_animation.h>

/* Byte view of GameTexturePalette::flags, matching its native bit accesses. */
struct FlowerPaletteFlags {
    u8 unknown_00_04 : 5, dirty : 1, buffered : 1, unknown_07 : 1;
};

extern "C" {
extern u16 data_ov014_020c6120[2];
void func_0202cc58(const void *source, void *destination, u32 size);
void func_0202cd2c(const void *source, void *destination, u32 size);

int Overlay14Participant_RenderFade(BattleSceneObject *object, int pass)
{
    if (pass)
        return 0;

    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    Overlay14Fade *fade = (Overlay14Fade *)object->render_override;
    model->prepare_render();
    u16 *palette = (u16 *)model->get_palette_buffer();
    FlowerPaletteFlags *flags = (FlowerPaletteFlags *)&model->render_palette.flags;
    if (!flags->dirty || !flags->buffered) {
        const void *source = model->get_palette_source();
        /* Select the native copy direction for possibly overlapping buffers. */
        if ((u32)source < (u32)palette)
            func_0202cd2c(source, palette, 512);
        else
            func_0202cc58(source, palette, 512);
    }

    int frame = fade->frame;
    if (!frame && fade->stop) {
        object->render_override = 0;
    } else {
        int color = data_ov014_020c6120[((Overlay14Work *)data_ov002_020c0710)->variant];
        /* A complete pulse lasts 16 calls; stop only at the unmodified color. */
        if (frame > 8)
            frame = 16 - frame;
        GamePaletteAnimation_ApplyEffect(-5, color, 13, frame << 8, palette, 256, 0, 0);
        if (++fade->frame >= 16)
            fade->frame = 0;
    }
    model->set_palette(palette);
    model->draw(0, 0, 0);
    return 1;
}
}
