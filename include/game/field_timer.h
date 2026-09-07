#ifndef PIT_GAME_FIELD_TIMER_H
#define PIT_GAME_FIELD_TIMER_H

#include <game/sprite_output.h>

typedef struct FieldTimerImage {
    u32 resource;
    u32 unknown_04;
    const void *tiles;
    u32 unknown_0c[2];
    u32 packed_tile_size;
} FieldTimerImage;

typedef struct FieldTimer {
    struct { u8 screen : 1, allocated : 1, paused : 1, unknown_3_7 : 5; } flags;
    s8 step, minutes, seconds, frames, hundredths;
    s16 x, y;
    u16 unknown_0a;
    const FieldTimerImage *image;
    GameSpriteAllocation allocation;
} FieldTimer;

typedef char FieldTimerImage_SizeCheck[sizeof(FieldTimerImage) == 24 ? 1 : -1];
typedef char FieldTimer_SizeCheck[sizeof(FieldTimer) == 40 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
FieldTimer *FieldTimer_Init(FieldTimer *timer);
void FieldTimer_Destroy(FieldTimer *timer);
void FieldTimer_SetImage(FieldTimer *timer, const FieldTimerImage *image);
void FieldTimer_SetValue(FieldTimer *timer, s8 minutes, s8 seconds, s8 frames);
void FieldTimer_SetVisible(FieldTimer *timer, u8 visible);
void FieldTimer_SetPosition(FieldTimer *timer, int screen, s16 x, s16 y);
void FieldTimer_Update(FieldTimer *timer);
#ifdef __cplusplus
}
#endif

#endif
