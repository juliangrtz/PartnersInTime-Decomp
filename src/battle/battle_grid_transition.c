#include <game/battle_context.h>
#include <game/battle_grid_transition.h>

enum BattleGridTransitionOffset {
    BATTLE_GRID_VIEW_X_OFFSET = 0x6760,
    BATTLE_GRID_VIEW_Y_OFFSET = 0x6762,
    BATTLE_GRID_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleGridTransitionFlag {
    BATTLE_GRID_TRANSITION_ACTIVE = 1 << 6
};

extern int BattleGridTransition_DrawPhaseA(
    BattleGridTransitionState *state);

void BattleGridTransition_UpdateTask(BattleGridTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleGridTransitionTask *BattleGridTransition_InitializeTask(
    BattleGridTransitionTask *task) {
    BattleGridTransitionState *state;

    task->state.angle = 0;
    state = &task->state;
    state->velocity = 0;
    state->remaining_frames = 128;
    task->callback = BattleGridTransition_UpdateTask;
    return task;
}

void BattleGridTransition_UpdateTask(BattleGridTransitionTask *task) {
    *(u16 *)(gBattleContext + BATTLE_GRID_VIEW_X_OFFSET) = 0;
    *(u16 *)(gBattleContext + BATTLE_GRID_VIEW_Y_OFFSET) = 0;
    if (BattleGridTransition_DrawPhaseA(&task->state) == 0) {
        *(u32 *)(gBattleContext + BATTLE_GRID_RUNTIME_FLAGS_OFFSET) &=
            ~BATTLE_GRID_TRANSITION_ACTIVE;
        task->callback = 0;
    }
}
