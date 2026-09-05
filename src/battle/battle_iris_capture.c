#include <game/battle_context.h>
#include <game/battle_display_capture.h>
#include <game/battle_iris_transition.h>

enum BattleIrisCaptureOffset {
    BATTLE_IRIS_CAPTURE_INTENSITY_OFFSET = 0x22,
    BATTLE_IRIS_CAPTURE_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleIrisCaptureConstant {
    BATTLE_IRIS_CAPTURE_DURATION = 64,
    BATTLE_IRIS_CAPTURE_BUSY_BIT = 5,
    BATTLE_IRIS_CAPTURE_ACTIVE = 1 << 6,
    BATTLE_IRIS_CAPTURE_CONFIGURED = 1 << 11
};

extern void BattleIrisCapture_Draw(int progress);

void BattleIrisCapture_WaitForResetTask(BattleIrisTransitionTask *task);
void BattleIrisCapture_UpdateTask(BattleIrisTransitionTask *task);
void BattleIrisCapture_BeginTask(BattleIrisTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
BattleIrisTransitionTask *BattleIrisCapture_InitializeTask(
    BattleIrisTransitionTask *task) {
    *(u16 *)(gBattleContext + BATTLE_IRIS_CAPTURE_INTENSITY_OFFSET) = 32;
    *(u32 *)(gBattleContext + BATTLE_IRIS_CAPTURE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_IRIS_CAPTURE_CONFIGURED;
    task->progress = 0;
    task->callback = BattleIrisCapture_BeginTask;
    return task;
}

void BattleIrisCapture_BeginTask(BattleIrisTransitionTask *task) {
    *(u16 *)(gBattleContext + BATTLE_IRIS_CAPTURE_INTENSITY_OFFSET) = 0;
    BattleDisplayCapture_QueueConfigure(0);
    task->callback = BattleIrisCapture_UpdateTask;
}

void BattleIrisCapture_UpdateTask(BattleIrisTransitionTask *task) {
    u32 flags =
        *(u32 *)(gBattleContext + BATTLE_IRIS_CAPTURE_RUNTIME_FLAGS_OFFSET);
    int progress;

    if (((flags << (31 - BATTLE_IRIS_CAPTURE_BUSY_BIT)) >> 31) == 0) {
        progress = ++task->progress;
        if (progress < BATTLE_IRIS_CAPTURE_DURATION) {
            BattleIrisCapture_Draw(BATTLE_IRIS_CAPTURE_DURATION - progress);
            return;
        }

        BattleDisplayCapture_QueueReset();
        *(u32 *)(gBattleContext +
                 BATTLE_IRIS_CAPTURE_RUNTIME_FLAGS_OFFSET) &=
            ~BATTLE_IRIS_CAPTURE_CONFIGURED;
        task->callback = BattleIrisCapture_WaitForResetTask;
    }
}

void BattleIrisCapture_WaitForResetTask(BattleIrisTransitionTask *task) {
    u32 flags =
        *(u32 *)(gBattleContext + BATTLE_IRIS_CAPTURE_RUNTIME_FLAGS_OFFSET);

    if (((flags << (31 - BATTLE_IRIS_CAPTURE_BUSY_BIT)) >> 31) == 0) {
        *(u32 *)(gBattleContext + BATTLE_IRIS_CAPTURE_RUNTIME_FLAGS_OFFSET) =
            flags & ~BATTLE_IRIS_CAPTURE_ACTIVE;
        task->callback = 0;
    }
}
