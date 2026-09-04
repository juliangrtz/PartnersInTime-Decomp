#include <game/battle_ai.h>

void BattleAI_StopScriptById(int script_id) {
    int packed_id = script_id;
    int task_type;

    switch (packed_id) {
    case 1:
        *(const void **)(gBattleContext + BATTLE_AI_PARTY_STATE_1_OFFSET) = 0;
        return;
    case 2:
        *(const void **)(gBattleContext + BATTLE_AI_PARTY_STATE_2_OFFSET) = 0;
        return;
    case 3:
        *(const void **)(gBattleContext + BATTLE_AI_PARTY_STATE_3_OFFSET) = 0;
        return;
    case 4:
        *(const void **)(gBattleContext + BATTLE_AI_PARTY_STATE_4_OFFSET) = 0;
        return;
    }

    task_type = packed_id & BATTLE_AI_TASK_TYPE_MASK;
    switch (task_type) {
    case BATTLE_AI_TASK_ACTION:
        BattleAITask_StopById(
            &gBattleAIActionTaskPool.active,
            packed_id & BATTLE_AI_TASK_ACTOR_ID_MASK
        );
        return;
    case BATTLE_AI_TASK_REACTION:
        BattleAITask_StopById(
            &gBattleAIReactionTaskPool.active,
            packed_id & BATTLE_AI_TASK_ACTOR_ID_MASK
        );
        return;
    case BATTLE_AI_TASK_AUXILIARY:
        BattleAITask_StopById(
            &gBattleAIAuxTaskPool.active,
            packed_id & BATTLE_AI_TASK_ACTOR_ID_MASK
        );
        return;
    case BATTLE_AI_TASK_OBJECT:
        BattleAITask_StopById(
            &gBattleAIObjectTaskPool.active,
            packed_id & BATTLE_AI_TASK_ACTOR_ID_MASK
        );
        return;
    }
}

void BattleAITask_StopById(BattleAITask **head, int actor_id) {
    BattleAITask *previous = *head;
    BattleAITask *task;

    if (previous == 0 || previous->id > actor_id) {
        return;
    }
    if (previous->id == actor_id) {
        if (previous->state != 0) {
            previous->state->script = 0;
        }
        BattleTask_Release(previous);
        return;
    }

    task = previous->next;
    while (task != 0) {
        if (task->id > actor_id) {
            return;
        }
        if (task->id == actor_id) {
            if (task->state != 0) {
                task->state->script = 0;
            }
            BattleTask_Release(previous->next);
            return;
        }
        previous = task;
        task = task->next;
    }
}
