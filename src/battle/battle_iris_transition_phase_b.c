/*
 * Iris transition, second phase (overlay 2, 0x020B5D04-0x020B5D58).
 *
 * The closing phase of the iris transition.
 */

#include <game/battle_transition.h>
#include <game/battle_ai.h>
#include <game/battle_iris_transition.h>

enum BattleIrisTransitionPhaseBConstant {
    BATTLE_IRIS_PHASE_B_DURATION = 64
};

extern void BattleIrisTransition_DrawPhaseB(int progress);

void BattleIrisTransitionPhaseB_UpdateTask(BattleIrisTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleIrisTransitionTask *BattleIrisTransitionPhaseB_InitializeTask(
    BattleIrisTransitionTask *task) {
    task->progress = 0;
    task->callback = BattleIrisTransitionPhaseB_UpdateTask;
    return task;
}

void BattleIrisTransitionPhaseB_UpdateTask(BattleIrisTransitionTask *task) {
    BattleIrisTransition_DrawPhaseB(task->progress);
    ++task->progress;
    if (task->progress >= BATTLE_IRIS_PHASE_B_DURATION) {
        BattleTransition_BeginReturn((BattleAITask *)task);
    }
}
