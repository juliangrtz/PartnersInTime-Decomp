#ifndef PIT_GAME_PAUSE_NAVIGATION_H
#define PIT_GAME_PAUSE_NAVIGATION_H
#include <game/pause_scene.h>

typedef struct PauseRumbleDelayTask {
    u8 unknown_00[32];
    s32 phase, delay;
    u8 unknown_28[32];
} PauseRumbleDelayTask;

typedef struct PausePageTask {
    u8 unknown_00[32];
    s32 phase, delay, pending, unknown_2c, queued_direction;
    u8 unknown_34[20];
} PausePageTask;

typedef char PauseRumbleDelayTaskSize[sizeof(PauseRumbleDelayTask) == 72 ? 1 : -1];
typedef char PausePageTaskSize[sizeof(PausePageTask) == 72 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
void PauseScene_DelayedRumbleTask(PauseRumbleDelayTask *task);
void PauseScene_ChooseEntryTask(PauseMenuElement *task);
void PausePage_CloseTask(PausePageTask *task);
void PausePage_OpenTask(PausePageTask *task);
#ifdef __cplusplus
}
#endif
#endif
