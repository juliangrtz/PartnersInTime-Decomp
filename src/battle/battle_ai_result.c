#include <game/battle_ai.h>

int BattleAI_HandleVmResult(BattleAITask *task, int result, BattleAIState *state) {
    switch (result) {
    case BATTLE_AI_VM_FINISHED:
        break;
    case BATTLE_AI_VM_CHAIN_SCRIPT:
        if (state->continuation_script != 0) {
            state->script = state->continuation_script;
            state->order = state->continuation_order;
            state->order_tie_break = state->continuation_tie_break;
            state->continuation_script = 0;
            state->continuation_order = 0;
            state->continuation_tie_break = 0xFFFF;
            return 1;
        }
        break;
    default:
        return 0;
    }

    task->callback = 0;
    state->order = 0;
    state->order_tie_break = 0xFFFF;
    state->continuation_script = 0;
    state->continuation_order = 0;
    state->continuation_tie_break = 0xFFFF;
    return 0;
}
