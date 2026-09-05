#include <game/battle_context.h>
#include <game/battle_background.h>
#include <game/battle_display_capture.h>
#include <game/battle_grid_transition.h>
#include <game/save_data.h>

enum BattleGridCaptureOffset {
    BATTLE_GRID_CAPTURE_SCENE_STATE_OFFSET = 0x18,
    BATTLE_GRID_CAPTURE_INTENSITY_OFFSET = 0x22,
    BATTLE_GRID_CAPTURE_RUNTIME_FLAGS_OFFSET = 0xD3A0,
    BATTLE_GRID_CAPTURE_SAVE_FLAGS_OFFSET = 0x560
};

enum BattleGridCaptureConstant {
    BATTLE_GRID_CAPTURE_REQUIRED_SCENE_STATE = 0x1002,
    BATTLE_GRID_CAPTURE_ALTERNATE_DIRECTION = 1 << 3,
    BATTLE_GRID_CAPTURE_BUSY = 1 << 5,
    BATTLE_GRID_CAPTURE_ACTIVE = 1 << 6,
    BATTLE_GRID_CAPTURE_CONFIGURED = 1 << 11
};

typedef struct BattleGridCaptureState {
    struct BattleGridCaptureSourceTask *source_task;
    u16 angle;
    u16 velocity;
    u16 started;
    u16 padding_0A;
} BattleGridCaptureState;

typedef struct BattleGridCaptureTask {
    struct BattleGridCaptureTask *next;
    void (*callback)(struct BattleGridCaptureTask *task);
    struct BattleGridCaptureTask **owner_slot;
    BattleGridCaptureState state;
} BattleGridCaptureTask;

typedef struct BattleGridCaptureSourceState {
    u32 unknown_00;
    u16 angle;
    u16 velocity;
    u16 remaining_frames;
} BattleGridCaptureSourceState;

typedef struct BattleGridCaptureSourceTask {
    struct BattleGridCaptureSourceTask *next;
    void (*callback)(struct BattleGridCaptureSourceTask *task);
    struct BattleGridCaptureSourceTask **owner_slot;
    BattleGridCaptureSourceState state;
} BattleGridCaptureSourceTask;

typedef char BattleGridCaptureState_SizeCheck[
    sizeof(BattleGridCaptureState) == 0x0C ? 1 : -1];
typedef char BattleGridCaptureTask_SizeCheck[
    sizeof(BattleGridCaptureTask) == 0x18 ? 1 : -1];
typedef char BattleGridCaptureSourceState_SizeCheck[
    sizeof(BattleGridCaptureSourceState) == 0x0C ? 1 : -1];
typedef char BattleGridCaptureSourceTask_SizeCheck[
    sizeof(BattleGridCaptureSourceTask) == 0x18 ? 1 : -1];

extern int BattleGridTransition_DrawPhaseB(BattleGridCaptureState *state);
extern void func_ov002_020b5d58(void *task);
extern void *func_ov002_02072508(
    void (*callback)(void *task), void *argument,
    int unknown_2, int unknown_3);

void BattleGridCapture_WaitForResetTask(BattleGridCaptureTask *task);
void BattleGridCapture_ResetTask(BattleGridCaptureTask *task);
void BattleGridCapture_UpdateTask(BattleGridCaptureTask *task);
void BattleGridCapture_BeginTask(BattleGridCaptureTask *task);
BattleGridCaptureTask *BattleGridCapture_InitializeTask(
    BattleGridCaptureTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleGridCaptureTask *BattleGridCapture_InitializeTask(
    BattleGridCaptureTask *task) {
    task->state.started = 0;
    task->callback = BattleGridCapture_BeginTask;
    return task;
}

void BattleGridCapture_BeginTask(BattleGridCaptureTask *task) {
    BattleGridCaptureState *state;
    BattleGridCaptureSourceState *source;

    if (*(u32 *)(gBattleContext + BATTLE_GRID_CAPTURE_SCENE_STATE_OFFSET) !=
        BATTLE_GRID_CAPTURE_REQUIRED_SCENE_STATE) {
        return;
    }

    source = &task->state.source_task->state;
    state = &task->state;
    if (source->remaining_frames != 0) {
        return;
    }

    BattleBackground_RequestToggle();
    state->started = 1;
    /* Preserve MWCC's single-bit extraction while keeping the save flag named. */
    if (((u32)*(u16 *)(gSaveData + BATTLE_GRID_CAPTURE_SAVE_FLAGS_OFFSET) <<
         28) >> 31 != 0) {
        state->angle = 0xC00;
    } else {
        state->angle = source->angle;
        state->velocity = source->velocity;
        if (((state->angle - 0x80) & 0xFFF) > 0x100) {
            state->angle += 0x800;
        }
    }

    *(u16 *)(gBattleContext + BATTLE_GRID_CAPTURE_INTENSITY_OFFSET) = 0;
    func_ov002_02072508(func_ov002_020b5d58, 0, 0, 0);
    BattleDisplayCapture_QueueConfigure(0);
    task->callback = BattleGridCapture_UpdateTask;
}

void BattleGridCapture_UpdateTask(BattleGridCaptureTask *task) {
    u32 flags =
        *(u32 *)(gBattleContext + BATTLE_GRID_CAPTURE_RUNTIME_FLAGS_OFFSET);

    if (((flags << 26) >> 31) == 0) {
        *(u16 *)(gBattleContext + BATTLE_GRID_CAPTURE_INTENSITY_OFFSET) = 32;
        if (BattleGridTransition_DrawPhaseB(&task->state) == 0) {
            task->callback = BattleGridCapture_ResetTask;
        }
    }
}

void BattleGridCapture_ResetTask(BattleGridCaptureTask *task) {
    BattleDisplayCapture_QueueReset();
    *(u32 *)(gBattleContext + BATTLE_GRID_CAPTURE_RUNTIME_FLAGS_OFFSET) &=
        ~BATTLE_GRID_CAPTURE_CONFIGURED;
    task->callback = BattleGridCapture_WaitForResetTask;
}

void BattleGridCapture_WaitForResetTask(BattleGridCaptureTask *task) {
    u32 flags =
        *(u32 *)(gBattleContext + BATTLE_GRID_CAPTURE_RUNTIME_FLAGS_OFFSET);

    if (((flags << 26) >> 31) == 0) {
        *(u32 *)(gBattleContext + BATTLE_GRID_CAPTURE_RUNTIME_FLAGS_OFFSET) =
            flags & ~BATTLE_GRID_CAPTURE_ACTIVE;
        task->callback = 0;
    }
}
