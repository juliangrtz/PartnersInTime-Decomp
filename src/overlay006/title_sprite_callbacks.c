#include <game/title_sprite_sequence.h>

extern const u8 data_ov006_0207b2d4[];
extern const s16 FX_SinCosTable_[];
int _s32_div_f(int, int);
void func_ov006_0206f710(TitleRotatingSprite *);

void func_ov006_0206f5cc(TitleRotatingSprite *);

void TitleRotatingSprite_Init(TitleRotatingSprite *work, TitleTextureResource *texture)
{
    work->sprite.texture = texture;
    work->sprite.layout = data_ov006_0207b2d4;
    work->sprite.header.update = (void (*)(void *))func_ov006_0206f710;
    work->sprite.header.draw = (void (*)(void *))func_ov006_0206f5cc;
    work->sprite.header.x = 0;
    work->sprite.header.y = 0;
    work->sprite.alpha = 31;
    work->sprite.scale_y = 4096;
    work->sprite.scale_x = work->sprite.scale_y;
    work->angle = 0;
    work->sprite.header.state = 0;
}

void TitleFadeSprite_Update(TitleFadeSprite *work)
{
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        ++work->header.elapsed;
        if (work->header.elapsed >= work->header.duration) {
            work->alpha = 31;
            work->header.state = 2;
        } else {
            work->alpha =
                31 *
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)] /
                4096;
        }
        break;
    case 2:
        break;
    }
}
