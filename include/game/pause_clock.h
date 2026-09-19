#ifndef PIT_GAME_PAUSE_CLOCK_H
#define PIT_GAME_PAUSE_CLOCK_H

/*
 * The play-time readout on the pause menu's status page.
 */
#include <nitro.h>

typedef struct PauseClockSeparatorTask {
    u8 unknown_00[36];
    int timer, visible;
    u8 unknown_2c[28];
} PauseClockSeparatorTask;
typedef char PauseClockSeparatorTaskSize[sizeof(PauseClockSeparatorTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void PauseClock_UpdateSeparator(PauseClockSeparatorTask *task);
#ifdef __cplusplus
}
#endif
#endif
