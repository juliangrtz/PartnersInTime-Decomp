#ifndef PIT_GAME_TITLE_BRIGHTNESS_H
#define PIT_GAME_TITLE_BRIGHTNESS_H
#include <nitro.h>

typedef struct TitleBrightness TitleBrightness;
struct TitleBrightness {
    void *next;
    s32 elapsed, duration;
    s32 x, y, depth;
    void (*update)(TitleBrightness *);
    void (*draw)(TitleBrightness *);
    void (*upload)(TitleBrightness *);
    u8 state;
    u8 draw_flags;
    u8 unknown_26;
    s8 start, target, current;
    struct {
        u8 sine_curve : 1, unknown_1_7 : 7;
    } flags;
    u8 unknown_2b;
};
typedef char TitleBrightness_SizeCheck[sizeof(TitleBrightness) == 44 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void TitleBrightness_Start(TitleBrightness *, int, int, int, int);
void TitleBrightness_Upload(TitleBrightness *);
void TitleBrightness_Update(TitleBrightness *);
void TitleBrightness_Evaluate(TitleBrightness *);
void TitleBrightness_Release(TitleBrightness *);
void TitleBrightness_Init(TitleBrightness *);
#ifdef __cplusplus
}
#endif
#endif
