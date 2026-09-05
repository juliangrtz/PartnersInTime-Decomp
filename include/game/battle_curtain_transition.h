#ifndef PIT_GAME_BATTLE_CURTAIN_TRANSITION_H
#define PIT_GAME_BATTLE_CURTAIN_TRANSITION_H

#include <nitro.h>

typedef struct BattleCurtainTransitionTask {
    struct BattleCurtainTransitionTask *next;
    void (*callback)(struct BattleCurtainTransitionTask *task);
    struct BattleCurtainTransitionTask **owner_slot;
    s16 progress;
    u16 padding_0E;
} BattleCurtainTransitionTask;

typedef char BattleCurtainTransitionTask_SizeCheck[
    sizeof(BattleCurtainTransitionTask) == 0x10 ? 1 : -1];

BattleCurtainTransitionTask *BattleCurtainTransition_InitializeTask(
    BattleCurtainTransitionTask *task);

#endif
