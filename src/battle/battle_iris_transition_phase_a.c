#include <game/battle_context.h>
#include <game/battle_iris_transition.h>

enum BattleIrisTransitionPhaseAOffset {
    BATTLE_IRIS_PHASE_A_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleIrisTransitionPhaseAConstant {
    BATTLE_IRIS_PHASE_A_DURATION = 64,
    BATTLE_IRIS_PHASE_A_ACTIVE = 1 << 6
};

extern void BattleIrisTransition_DrawPhaseA(int progress);

void BattleIrisTransitionPhaseA_UpdateTask(BattleIrisTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleIrisTransitionTask *BattleIrisTransitionPhaseA_InitializeTask(
    BattleIrisTransitionTask *task) {
    task->progress = 0;
    task->callback = BattleIrisTransitionPhaseA_UpdateTask;
    return task;
}

void BattleIrisTransitionPhaseA_UpdateTask(BattleIrisTransitionTask *task) {
    BattleIrisTransition_DrawPhaseA(task->progress);
    ++task->progress;
    if (task->progress >= BATTLE_IRIS_PHASE_A_DURATION) {
        *(u32 *)(gBattleContext + BATTLE_IRIS_PHASE_A_RUNTIME_FLAGS_OFFSET) &=
            ~BATTLE_IRIS_PHASE_A_ACTIVE;
        task->callback = 0;
    }
}
