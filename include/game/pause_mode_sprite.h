#ifndef PIT_GAME_PAUSE_MODE_SPRITE_H
#define PIT_GAME_PAUSE_MODE_SPRITE_H

#include <nitro.h>

/* A pooled sprite task that remains active while its mode matches the scene. */
typedef struct PauseModeSpriteTask {
    u8 unknown_00[32];
    int state, counter;
    s16 mode, unknown_2a, anchor_y, unknown_2e;
    fx32 x, y, scale;
    int unknown_3c, unknown_40;
    fx32 step;
} PauseModeSpriteTask;

typedef char PauseModeSpriteTaskSizeCheck[sizeof(PauseModeSpriteTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void PauseModeSprite_Update(PauseModeSpriteTask *task);
void PauseModeSprite_UpdateTimed(PauseModeSpriteTask *task);
#ifdef __cplusplus
}
#endif

#endif
