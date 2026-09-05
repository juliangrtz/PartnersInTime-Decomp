#include <game/battle_ai.h>
#include <game/battle_iris_transition.h>

enum BattleIrisTransitionPhaseBConstant {
    BATTLE_IRIS_PHASE_B_DURATION = 64
};

extern void BattleIrisTransition_DrawPhaseB(int progress);
extern BattleAITask *func_ov002_020b5ef0(BattleAITask *task);

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
        func_ov002_020b5ef0((BattleAITask *)task);
    }
}
