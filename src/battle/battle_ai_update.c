#include <game/battle_ai.h>

BattleAITask *BattleAITask_GetOrInsert(BattleTaskPool *pool, int actor_id) {
    BattleAITask *task = pool->active;
    BattleAITask *next;

    if (task != 0) {
        if (task->id > actor_id) {
            return BattleTaskList_Insert(pool, 0);
        }
        if (task->id == actor_id) {
            return task;
        }

        for (next = task->next; next != 0; next = next->next) {
            if (next->id > actor_id) {
                BattleAITask *inserted = BattleTaskPool_Allocate(pool);

                inserted->next = task->next;
                task->next = inserted;
                return task->next;
            }
            if (next->id == actor_id) {
                return next;
            }
            task = next;
        }

        next = BattleTaskPool_Allocate(pool);
        next->next = 0;
        task->next = next;
        return task->next;
    }

    return BattleTaskList_Insert(pool, 0);
}

void BattleAI_UpdateActionTask(BattleAITask *task) {
    BattleAIState *state = task->state;
    int result;

    if (state->disabled) {
        return;
    }
    if (state->wait_for_order && !BattleAI_TryClearOrderWait(state)) {
        return;
    }

    result = VM_Run(
        (ScriptVm *)(gBattleContext + BATTLE_AI_VM_OFFSET),
        (ScriptVmState *)state
    );
    switch (result) {
    case BATTLE_AI_VM_FINISHED:
    case BATTLE_AI_VM_CHAIN_SCRIPT:
        task->callback = 0;
        break;
    }
}

void BattleAI_UpdateReactionTask(BattleAITask *task) {
    BattleAIState *state = task->state;
    int result;

    if (state->disabled) {
        return;
    }
    if (state->wait_for_order && !BattleAI_TryClearOrderWait(state)) {
        return;
    }

    result = VM_Run(
        (ScriptVm *)(gBattleContext + BATTLE_AI_VM_OFFSET),
        (ScriptVmState *)state
    );
    switch (result) {
    case BATTLE_AI_VM_FINISHED:
    case BATTLE_AI_VM_CHAIN_SCRIPT:
        task->callback = 0;
        break;
    }
}

void BattleAI_UpdateAuxTask(BattleAITask *task) {
    BattleAIState *state = task->state;
    int result;

    if (state->disabled) {
        return;
    }

    result = VM_Run(
        (ScriptVm *)(gBattleContext + BATTLE_AI_VM_OFFSET),
        (ScriptVmState *)state
    );
    switch (result) {
    case BATTLE_AI_VM_FINISHED:
    case BATTLE_AI_VM_CHAIN_SCRIPT:
        task->callback = 0;
        break;
    }
}

void BattleAI_UpdateChainedTask(BattleAITask *task) {
    BattleAIState *state = task->state;

    if (state->script == 0) {
        task->callback = 0;
        state->order = 0;
        state->order_tie_break = 0xFFFF;
        state->continuation_script = 0;
        state->continuation_order = 0;
        state->continuation_tie_break = 0xFFFF;
        return;
    }
    if (state->disabled) {
        return;
    }

    do {
        int result = VM_Run(
            (ScriptVm *)(gBattleContext + BATTLE_AI_VM_OFFSET),
            (ScriptVmState *)state
        );

        if (!BattleAI_HandleVmResult(task, result, state)) {
            return;
        }
    } while (1);
}
