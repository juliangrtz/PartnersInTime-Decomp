#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_grid_transition.h>

enum BattleGridCaptureSourceOffset {
    BATTLE_GRID_CAPTURE_SOURCE_TASK_POOL_OFFSET = 0x8B44
};

extern int BattleGridTransition_DrawCaptureSource(
    BattleGridCaptureSourceState *state);

void BattleGridCaptureSource_UpdateTask(BattleGridCaptureSourceTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleGridCaptureTask *BattleGridCaptureSource_InitializeTask(
    BattleGridCaptureSourceTask *task) {
    BattleGridCaptureSourceState *state = &task->state;

    state->angle = 0;
    state->velocity = 0;
    state->remaining_frames = 0;
    state->capture_task = (BattleGridCaptureTask *)BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext +
                           BATTLE_GRID_CAPTURE_SOURCE_TASK_POOL_OFFSET),
        0);
    state->capture_task->state.source_task = task;
    task->callback = BattleGridCaptureSource_UpdateTask;
    return BattleGridCapture_InitializeTask(state->capture_task);
}

void BattleGridCaptureSource_UpdateTask(BattleGridCaptureSourceTask *task) {
    BattleGridCaptureSourceState *state = &task->state;

    if (state->capture_task->state.started != 0) {
        task->callback = 0;
        return;
    }
    BattleGridTransition_DrawCaptureSource(state);
}
