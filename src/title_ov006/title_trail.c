#include "title_trail_internal.h"
void *func_020365a4(void), *func_0203655c(void);
void func_02018ce0(void *, int, int, int);
void func_02018d4c(const void *, int, int, int, int, void *, int, int);
void func_0202ce08(const void *, void *, int, int);

void TitleTrailStamp_Draw(void *element)
{
    TitleTrailStamp *work = element;
    int top = work->header.y / 4096 - ((u32)work->height >> 1);
    int bottom = work->header.y / 4096 + ((u32)work->height >> 1);
    int left = work->header.x / 4096 - (work->width_words * 8 / 2);
    int right = work->header.x / 4096 + (work->width_words * 8 / 2);
    if (((top >= 0 && top < 192) || (bottom >= 0 && bottom < 192)) &&
        ((left >= 0 && left < 256) || (right >= 0 && right < 256)))
        func_02018d4c(work->pixels, work->width_words, work->height, left, top,
                       work->buffers->main_pixels, 32, 192);
    if (((top - 244 >= 0 && top - 244 < 192) || (bottom - 244 >= 0 && bottom - 244 < 192)) &&
        ((left >= 0 && left < 256) || (right >= 0 && right < 256)))
        func_02018d4c(work->pixels, work->width_words, work->height, left, top - 244,
                       work->buffers->sub_pixels, 32, 192);
}
void TitleTrailStamp_Init(TitleTrailStamp *work, TitleTrailBuffers *buffers,
                        const void *pixels, int width_words, int height, void (*update)(void *))
{
    work->buffers = buffers;
    work->header.update = update;
    work->pixels = pixels;
    work->width_words = width_words;
    work->height = height;
}
void TitleTrailStamp_Release(TitleTrailStamp *work)
{
}
void TitleTrailBuffers_Update(void *element)
{
    TitleTrailBuffers *work = element;
    func_02018ce0(work->main_pixels, 32, 192, 14);
    func_02018ce0(work->sub_pixels, 32, 192, 14);
}
void TitleTrailBuffers_Upload(void *element)
{
    TitleTrailBuffers *work = element;
    func_0202ce08(work->main_pixels, func_020365a4(), 128, 192);
    func_0202ce08(work->sub_pixels, func_0203655c(), 128, 192);
}
void TitleTrailBuffers_Init(TitleTrailBuffers *work, void *main_pixels, void *sub_pixels)
{
    work->header.update = TitleTrailBuffers_Update;
    work->header.upload = TitleTrailBuffers_Upload;
    work->main_pixels = main_pixels;
    work->sub_pixels = sub_pixels;
}
void TitleTrailBuffers_Release(TitleTrailBuffers *work)
{
}
