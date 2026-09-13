#ifndef PIT_GAME_PAUSE_NUMBERS_H
#define PIT_GAME_PAUSE_NUMBERS_H

#include <nitro.h>

enum PauseNumberKind {
    PAUSE_NUMBER_LEVEL = 0,
    PAUSE_NUMBER_CURRENT_HP = 1,
    PAUSE_NUMBER_MAX_HP = 2,
    PAUSE_NUMBER_COINS = 3,
    PAUSE_NUMBER_HOURS = 4,
    PAUSE_NUMBER_MINUTES = 5
};

enum PauseNumberState {
    PAUSE_NUMBER_INITIALIZE = 0,
    PAUSE_NUMBER_IDLE = 10,
    PAUSE_NUMBER_INTERPOLATE = 11
};

typedef struct PauseNumberTask {
    u8 unknown_00[32];
    int state, counter;
    u8 member : 7;
    u8 leading_zeroes : 1;
    u8 kind, palette, digits;
    int last_value, divisor;
    s16 x, y;
    int current_q8, target_q8, step_q8;
    u8 unknown_44[4];
} PauseNumberTask;
typedef char PauseNumberTaskSize[sizeof(PauseNumberTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

/* kind must be a PauseNumberKind; party kinds use member indices 0..3. */
PauseNumberTask *PauseNumber_Create(int x, int y, int member, int kind, int digits, int leading_zeroes,
                                   int group);
void PauseNumber_Update(PauseNumberTask *task);

#ifdef __cplusplus
}
#endif
#endif
