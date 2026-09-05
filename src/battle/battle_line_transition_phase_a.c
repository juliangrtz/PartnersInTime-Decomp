#include <game/battle_context.h>
#include <game/battle_line_transition.h>

enum BattleLineTransitionPhaseAOffset {
    BATTLE_LINE_PHASE_A_POSITIONS_OFFSET = 0x5F688,
    BATTLE_LINE_PHASE_A_VELOCITIES_OFFSET = 0x5F6C8,
    BATTLE_LINE_PHASE_A_VIEW_X_OFFSET = 0x6760,
    BATTLE_LINE_PHASE_A_VIEW_Y_OFFSET = 0x6762,
    BATTLE_LINE_PHASE_A_RUNTIME_FLAGS_OFFSET = 0xD3A0
};

enum BattleLineTransitionPhaseAConstant {
    BATTLE_LINE_PHASE_A_LINE_COUNT = 32,
    BATTLE_LINE_PHASE_A_INITIAL_VELOCITY = -10,
    BATTLE_LINE_PHASE_A_ACTIVE = 1 << 6
};

extern void func_0202cbd4(void *destination, int value, u32 size);
extern int BattleLineTransition_DrawPhaseA(BattleLineTransitionState *state);

void BattleLineTransitionPhaseA_UpdateTask(BattleLineTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
void BattleLineTransitionPhaseA_InitializeTask(
    BattleLineTransitionTask *task) {
    BattleLineTransitionState *state;

    task->state.positions =
        (s16 *)(gBattleContext + BATTLE_LINE_PHASE_A_POSITIONS_OFFSET);
    state = &task->state;
    state->velocities =
        (s16 *)(gBattleContext + BATTLE_LINE_PHASE_A_VELOCITIES_OFFSET);
    state->frame = 0;
    func_0202cbd4(state->positions, 0,
                  BATTLE_LINE_PHASE_A_LINE_COUNT * sizeof(s16));
    func_0202cbd4(state->velocities,
                  (u16)BATTLE_LINE_PHASE_A_INITIAL_VELOCITY |
                      ((u32)(u16)BATTLE_LINE_PHASE_A_INITIAL_VELOCITY << 16),
                  BATTLE_LINE_PHASE_A_LINE_COUNT * sizeof(s16));
    task->callback = BattleLineTransitionPhaseA_UpdateTask;
}

void BattleLineTransitionPhaseA_UpdateTask(BattleLineTransitionTask *task) {
    BattleLineTransitionState *state = &task->state;

    *(u16 *)(gBattleContext + BATTLE_LINE_PHASE_A_VIEW_X_OFFSET) = 0;
    *(u16 *)(gBattleContext + BATTLE_LINE_PHASE_A_VIEW_Y_OFFSET) = 0;
    ++state->frame;
    if (BattleLineTransition_DrawPhaseA(state) == 0) {
        *(u32 *)(gBattleContext + BATTLE_LINE_PHASE_A_RUNTIME_FLAGS_OFFSET) &=
            ~BATTLE_LINE_PHASE_A_ACTIVE;
        task->callback = 0;
    }
}
