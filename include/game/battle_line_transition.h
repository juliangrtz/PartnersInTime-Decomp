#ifndef PIT_GAME_BATTLE_LINE_TRANSITION_H
#define PIT_GAME_BATTLE_LINE_TRANSITION_H

#include <nitro.h>

typedef struct BattleLineTransitionState {
    s16 *positions;
    s16 *velocities;
    s16 frame;
    u16 padding_0A;
} BattleLineTransitionState;

typedef struct BattleLineTransitionTask {
    struct BattleLineTransitionTask *next;
    void (*callback)(struct BattleLineTransitionTask *task);
    struct BattleLineTransitionTask **owner_slot;
    BattleLineTransitionState state;
} BattleLineTransitionTask;

typedef char BattleLineTransitionState_SizeCheck[
    sizeof(BattleLineTransitionState) == 0x0C ? 1 : -1];
typedef char BattleLineTransitionTask_SizeCheck[
    sizeof(BattleLineTransitionTask) == 0x18 ? 1 : -1];

void BattleLineTransitionPhaseA_InitializeTask(
    BattleLineTransitionTask *task);
void BattleLineCapture_InitializeTask(BattleLineTransitionTask *task);
void BattleLineTransitionPhaseB_InitializeTask(
    BattleLineTransitionTask *task);

#endif
