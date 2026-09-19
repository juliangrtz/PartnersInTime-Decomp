#include <game/title_brightness.h>
#include <nitro/gx_init.h>
extern const s16 FX_SinCosTable_[];
int _s32_div_f(int, int);

void TitleBrightness_Init(TitleBrightness *work)
{
    work->update = TitleBrightness_Update;
    work->upload = TitleBrightness_Upload;
    work->state = 0;
    work->draw_flags = 255;
    work->start = 0;
    work->target = 16;
    work->current = 16;
    work->flags.sine_curve = 0;
}

void TitleBrightness_Release(TitleBrightness *work)
{
    work->update = 0;
    work->upload = 0;
}

void TitleBrightness_Evaluate(TitleBrightness *work)
{
    int delta;
    int start = work->start;
    int factor, brightness;
    delta = work->target - start;
    switch (work->flags.sine_curve) {
    case 0:
        factor = _s32_div_f(work->elapsed << 12, work->duration);
        break;
    case 1:
        factor = FX_SinCosTable_[2 * (_s32_div_f(work->elapsed << 14, work->duration) >> 4)];
        break;
    }
    brightness = start + delta * factor / 4096;
    if (brightness < -16)
        brightness = -16;
    if (brightness > 16)
        brightness = 16;
    work->current = brightness;
}

void TitleBrightness_Update(TitleBrightness *work)
{
    if (work->elapsed >= work->duration) {
        switch (work->state) {
        case 0:
            break;
        case 1:
            work->current = work->target;
            work->state = 2;
            break;
        case 2:
            work->state = 0;
            work->draw_flags = 255;
            break;
        }
    } else {
        TitleBrightness_Evaluate(work);
        ++work->elapsed;
    }
}

void TitleBrightness_Upload(TitleBrightness *work)
{
    GXi_SetMasterBrightness((vu16 *)0x0400006c, work->current);
    GXi_SetMasterBrightness((vu16 *)0x0400106c, work->current);
}

void TitleBrightness_Start(TitleBrightness *work, int duration, int start, int target, int sine_curve)
{
    work->elapsed = _s32_div_f(duration * (work->current - start), target - start);
    work->duration = duration;
    work->start = start;
    work->target = target;
    work->flags.sine_curve = (u8)sine_curve;
    work->draw_flags = 0;
    TitleBrightness_Evaluate(work);
    work->state = 1;
}
