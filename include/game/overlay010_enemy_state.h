#ifndef PARTNERS_IN_TIME_GAME_OVERLAY010_ENEMY_STATE_H
#define PARTNERS_IN_TIME_GAME_OVERLAY010_ENEMY_STATE_H

#include <nitro.h>

enum Overlay10EnemyConstant {
    OVERLAY10_ENEMY_COUNT = 6
};

typedef struct Overlay10EnemyState {
    u8 unknown_000[0xDE];
    s16 current_hp[OVERLAY10_ENEMY_COUNT];
    s16 position_z[OVERLAY10_ENEMY_COUNT];
    u16 scale_steps[OVERLAY10_ENEMY_COUNT];
} Overlay10EnemyState;

#endif
