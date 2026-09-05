#include <game/battle_context.h>
#include <game/battle_curtain_transition.h>

enum BattleCurtainTransitionOffset {
    BATTLE_CURTAIN_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleCurtainTransitionFlag {
    BATTLE_CURTAIN_ACTIVE = 1 << 6
};

extern void BattleCurtainTransition_Draw(int progress);

void BattleCurtainTransition_UpdateTask(BattleCurtainTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleCurtainTransitionTask *BattleCurtainTransition_InitializeTask(
    BattleCurtainTransitionTask *task) {
    task->progress = 0;
    task->callback = BattleCurtainTransition_UpdateTask;
    return task;
}

void BattleCurtainTransition_UpdateTask(BattleCurtainTransitionTask *task) {
    BattleCurtainTransition_Draw(task->progress * 2);
    ++task->progress;
    if (task->progress >= 32) {
        *(u32 *)(gBattleContext + BATTLE_CURTAIN_RUNTIME_FLAGS_OFFSET) &=
            ~BATTLE_CURTAIN_ACTIVE;
        task->callback = 0;
    }
}
