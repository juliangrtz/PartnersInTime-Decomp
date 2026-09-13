#ifndef PIT_GAME_PAUSE_PARTY_BITMAP_H
#define PIT_GAME_PAUSE_PARTY_BITMAP_H

#include <nitro.h>

enum PausePartyValueKind {
    PAUSE_PARTY_VALUE_LEVEL = 0,
    PAUSE_PARTY_VALUE_CURRENT_HP = 1,
    PAUSE_PARTY_VALUE_MAX_HP = 2
};

enum PausePartyTransitionMode {
    PAUSE_PARTY_TRANSITION_CLOTHING = 0,
    PAUSE_PARTY_TRANSITION_BADGES = 1,
    PAUSE_PARTY_TRANSITION_STATUS = 2
};

typedef struct PausePartyTransitionTask {
    u8 unknown_00[32];
    int phase, unknown_24, kind, column, step;
    u8 unknown_34[20];
} PausePartyTransitionTask;
typedef char PausePartyTransitionTaskSize[sizeof(PausePartyTransitionTask) == 72 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif

/* Switch the four party bitmaps between equipment and ordinary status. */
void PausePartyBitmap_StartTransition(int mode);
void PausePartyBitmap_UpdateTransition(PausePartyTransitionTask *task);

/* Rebuild one member's 112-by-40 bitmap within the four-member buffer. */
void PausePartyBitmap_Rebuild(int member);
/* kind must be one of PausePartyValueKind; digits includes leading blank places. */
void PausePartyBitmap_DrawValue(int x, int y, int member, int kind, int digits, int leading_zeroes);

#ifdef __cplusplus
}
#endif
#endif
