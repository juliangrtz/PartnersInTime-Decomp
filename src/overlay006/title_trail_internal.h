#ifndef PIT_TITLE_TRAIL_INTERNAL_H
#define PIT_TITLE_TRAIL_INTERNAL_H
#include <game/title_sprite_sequence.h>

typedef struct TitleTrailBuffers {
    TitleSpriteHeader header;
    void *main_pixels, *sub_pixels;
    u32 unknown_30;
} TitleTrailBuffers;

typedef struct TitleTrailStamp {
    TitleSpriteHeader header;
    TitleTrailBuffers *buffers;
    const void *pixels;
    u16 width_words, height;
} TitleTrailStamp;

typedef char TitleTrailBuffersSize[sizeof(TitleTrailBuffers) == 52 ? 1 : -1];
typedef char TitleTrailStampSize[sizeof(TitleTrailStamp) == 52 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void TitleTrailBuffers_Release(TitleTrailBuffers *work);
void TitleTrailBuffers_Init(TitleTrailBuffers *work, void *main_pixels, void *sub_pixels);
void TitleTrailBuffers_Upload(void *element);
void TitleTrailBuffers_Update(void *element);
void TitleTrailStamp_Release(TitleTrailStamp *work);
void TitleTrailStamp_Init(TitleTrailStamp *work, TitleTrailBuffers *buffers, const void *pixels,
                          int width_words, int height, void (*update)(void *));
void TitleTrailStamp_Draw(void *element);
#ifdef __cplusplus
}
#endif
#endif
