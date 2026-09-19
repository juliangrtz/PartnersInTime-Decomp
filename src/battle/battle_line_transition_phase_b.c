/*
 * Line transition, second phase (overlay 2, 0x020B3714-0x020B37CC).
 *
 * The closing phase of the line transition.
 */

#include <game/battle_transition.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_line_transition.h>

enum BattleLineTransitionPhaseBOffset {
    BATTLE_LINE_PHASE_B_POSITIONS_OFFSET = 0x5F688,
    BATTLE_LINE_PHASE_B_VELOCITIES_OFFSET = 0x5F6C8
};

enum BattleLineTransitionPhaseBConstant {
    BATTLE_LINE_PHASE_B_LINE_COUNT = 32,
    BATTLE_LINE_PHASE_B_INITIAL_VELOCITY = -13
};

extern void func_0202cbd4(void *destination, int value, u32 size);
extern int BattleLineTransition_DrawPhaseB(BattleLineTransitionState *state);

void BattleLineTransitionPhaseB_UpdateTask(BattleLineTransitionTask *task);

/* Metrowerks emits C functions in reverse source order. */
void BattleLineTransitionPhaseB_InitializeTask(
    BattleLineTransitionTask *task) {
    BattleLineTransitionState *state;

    task->state.positions =
        (s16 *)(gBattleContext + BATTLE_LINE_PHASE_B_POSITIONS_OFFSET);
    state = &task->state;
    state->velocities =
        (s16 *)(gBattleContext + BATTLE_LINE_PHASE_B_VELOCITIES_OFFSET);
    state->frame = 0;
    func_0202cbd4(state->positions, 0,
                  BATTLE_LINE_PHASE_B_LINE_COUNT * sizeof(s16));
    func_0202cbd4(state->velocities,
                  (u16)BATTLE_LINE_PHASE_B_INITIAL_VELOCITY |
                      ((u32)(u16)BATTLE_LINE_PHASE_B_INITIAL_VELOCITY << 16),
                  BATTLE_LINE_PHASE_B_LINE_COUNT * sizeof(s16));
    task->callback = BattleLineTransitionPhaseB_UpdateTask;
}

void BattleLineTransitionPhaseB_UpdateTask(BattleLineTransitionTask *task) {
    BattleLineTransitionState *state = &task->state;

    ++state->frame;
    if (BattleLineTransition_DrawPhaseB(state) == 0) {
        BattleTransition_BeginReturn((BattleAITask *)task);
    }
}
