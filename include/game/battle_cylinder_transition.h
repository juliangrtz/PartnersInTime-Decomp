#ifndef PIT_GAME_BATTLE_CYLINDER_TRANSITION_H
#define PIT_GAME_BATTLE_CYLINDER_TRANSITION_H

/*
 * The transition that wraps the captured screen onto a rotating cylinder. Its
 * per-scanline work runs in a VCount interrupt, so the state here is read by both
 * the task and the handler.
 */

#include <nitro.h>

typedef struct BattleCylinderTransitionState {
    s16 progress;
    s16 angle;
} BattleCylinderTransitionState;

typedef struct BattleCylinderTransitionTask {
    struct BattleCylinderTransitionTask *next;
    void (*callback)(struct BattleCylinderTransitionTask *task);
    struct BattleCylinderTransitionTask **owner_slot;
    BattleCylinderTransitionState state;
} BattleCylinderTransitionTask;

typedef char BattleCylinderTransitionState_SizeCheck[
    sizeof(BattleCylinderTransitionState) == 4 ? 1 : -1];
typedef char BattleCylinderTransitionTask_SizeCheck[
    sizeof(BattleCylinderTransitionTask) == 0x10 ? 1 : -1];

void BattleCylinderTransition_InitializeTask(
    BattleCylinderTransitionTask *task);

#endif
