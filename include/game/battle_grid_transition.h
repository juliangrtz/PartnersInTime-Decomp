#ifndef PIT_GAME_BATTLE_GRID_TRANSITION_H
#define PIT_GAME_BATTLE_GRID_TRANSITION_H

#include <nitro.h>

typedef struct BattleGridTransitionState {
    u16 angle;
    u16 velocity;
    u16 remaining_frames;
} BattleGridTransitionState;

typedef struct BattleGridTransitionTask {
    struct BattleGridTransitionTask *next;
    void (*callback)(struct BattleGridTransitionTask *task);
    struct BattleGridTransitionTask **owner_slot;
    BattleGridTransitionState state;
} BattleGridTransitionTask;

typedef char BattleGridTransitionState_SizeCheck[
    sizeof(BattleGridTransitionState) == 6 ? 1 : -1];
typedef char BattleGridTransitionTask_SizeCheck[
    sizeof(BattleGridTransitionTask) == 0x14 ? 1 : -1];

BattleGridTransitionTask *BattleGridTransition_InitializeTask(
    BattleGridTransitionTask *task);

#endif
