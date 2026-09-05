#ifndef PIT_GAME_BATTLE_IRIS_TRANSITION_H
#define PIT_GAME_BATTLE_IRIS_TRANSITION_H

#include <nitro.h>

typedef struct BattleIrisTransitionTask {
    struct BattleIrisTransitionTask *next;
    void (*callback)(struct BattleIrisTransitionTask *task);
    struct BattleIrisTransitionTask **owner_slot;
    s16 progress;
    u16 padding_0E;
} BattleIrisTransitionTask;

typedef char BattleIrisTransitionTask_SizeCheck[
    sizeof(BattleIrisTransitionTask) == 0x10 ? 1 : -1];

BattleIrisTransitionTask *BattleIrisTransitionPhaseA_InitializeTask(
    BattleIrisTransitionTask *task);
BattleIrisTransitionTask *BattleIrisCapture_InitializeTask(
    BattleIrisTransitionTask *task);
BattleIrisTransitionTask *BattleIrisTransitionPhaseB_InitializeTask(
    BattleIrisTransitionTask *task);

#endif
