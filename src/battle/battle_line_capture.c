#include <game/battle_context.h>
#include <game/battle_display_capture.h>
#include <game/battle_line_transition.h>

enum BattleLineCaptureOffset {
    BATTLE_LINE_CAPTURE_POSITIONS_OFFSET = 0x5F688,
    BATTLE_LINE_CAPTURE_VELOCITIES_OFFSET = 0x5F6C8,
    BATTLE_LINE_CAPTURE_INTENSITY_OFFSET = 0x22,
    BATTLE_LINE_CAPTURE_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleLineCaptureConstant {
    BATTLE_LINE_CAPTURE_LINE_COUNT = 32,
    BATTLE_LINE_CAPTURE_INITIAL_VELOCITY = -13,
    BATTLE_LINE_CAPTURE_BUSY_BIT = 5,
    BATTLE_LINE_CAPTURE_ACTIVE = 1 << 6,
    BATTLE_LINE_CAPTURE_CONFIGURED = 1 << 11
};

extern void func_0202cbd4(void *destination, int value, u32 size);
extern int BattleLineCapture_Draw(BattleLineTransitionState *state);

void BattleLineCapture_WaitForResetTask(BattleLineTransitionTask *task);
void BattleLineCapture_ResetTask(BattleLineTransitionTask *task);
void BattleLineCapture_UpdateTask(BattleLineTransitionTask *task);
void BattleLineCapture_BeginTask(BattleLineTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
void BattleLineCapture_InitializeTask(BattleLineTransitionTask *task) {
    BattleLineTransitionState *state;

    *(u16 *)(gBattleContext + BATTLE_LINE_CAPTURE_INTENSITY_OFFSET) = 32;
    *(u32 *)(gBattleContext + BATTLE_LINE_CAPTURE_RUNTIME_FLAGS_OFFSET) |=
        BATTLE_LINE_CAPTURE_CONFIGURED;
    task->state.positions =
        (s16 *)(gBattleContext + BATTLE_LINE_CAPTURE_POSITIONS_OFFSET);
    state = &task->state;
    state->velocities =
        (s16 *)(gBattleContext + BATTLE_LINE_CAPTURE_VELOCITIES_OFFSET);
    state->frame = 0;
    func_0202cbd4(state->positions, 0,
                  BATTLE_LINE_CAPTURE_LINE_COUNT * sizeof(s16));
    func_0202cbd4(state->velocities,
                  (u16)BATTLE_LINE_CAPTURE_INITIAL_VELOCITY |
                      ((u32)(u16)BATTLE_LINE_CAPTURE_INITIAL_VELOCITY << 16),
                  BATTLE_LINE_CAPTURE_LINE_COUNT * sizeof(s16));
    task->callback = BattleLineCapture_BeginTask;
}

void BattleLineCapture_BeginTask(BattleLineTransitionTask *task) {
    *(u16 *)(gBattleContext + BATTLE_LINE_CAPTURE_INTENSITY_OFFSET) = 0;
    BattleDisplayCapture_QueueConfigure(0);
    task->callback = BattleLineCapture_UpdateTask;
}

void BattleLineCapture_UpdateTask(BattleLineTransitionTask *task) {
    u32 flags =
        *(u32 *)(gBattleContext + BATTLE_LINE_CAPTURE_RUNTIME_FLAGS_OFFSET);
    BattleLineTransitionState *state;

    if (((flags << (31 - BATTLE_LINE_CAPTURE_BUSY_BIT)) >> 31) == 0) {
        *(u16 *)(gBattleContext + BATTLE_LINE_CAPTURE_INTENSITY_OFFSET) = 32;
        state = &task->state;
        ++state->frame;
        if (BattleLineCapture_Draw(state) == 0) {
            task->callback = BattleLineCapture_ResetTask;
        }
    }
}

void BattleLineCapture_ResetTask(BattleLineTransitionTask *task) {
    BattleDisplayCapture_QueueReset();
    *(u32 *)(gBattleContext + BATTLE_LINE_CAPTURE_RUNTIME_FLAGS_OFFSET) &=
        ~BATTLE_LINE_CAPTURE_CONFIGURED;
    task->callback = BattleLineCapture_WaitForResetTask;
}

void BattleLineCapture_WaitForResetTask(BattleLineTransitionTask *task) {
    u32 flags =
        *(u32 *)(gBattleContext + BATTLE_LINE_CAPTURE_RUNTIME_FLAGS_OFFSET);

    if (((flags << (31 - BATTLE_LINE_CAPTURE_BUSY_BIT)) >> 31) == 0) {
        *(u32 *)(gBattleContext + BATTLE_LINE_CAPTURE_RUNTIME_FLAGS_OFFSET) =
            flags & ~BATTLE_LINE_CAPTURE_ACTIVE;
        task->callback = 0;
    }
}
