#include <game/battle_ai.h>

extern void func_ov002_020698cc(ScriptVm *vm, ScriptVmState *state);

void BattleAI_TaskPoolsInit(void) {
    BattleTaskPool_Init(&gBattleAIActionTaskPool, 8, 8);
    BattleTaskPool_Init(&gBattleAIReactionTaskPool, 8, 8);
    BattleTaskPool_Init(&gBattleAIAuxTaskPool, 8, 8);
    BattleTaskPool_Init(&gBattleAIObjectTaskPool, 40, 8);
}

void BattleAI_UpdateAll(void) {
    u8 *context = gBattleContext;
    BattleAIState *state;

    if (context == 0) {
        return;
    }

    state = (BattleAIState *)(context + BATTLE_AI_PARTY_STATE_1_OFFSET);
    if (state->script != 0) {
        VM_Run((ScriptVm *)(context + BATTLE_AI_VM_OFFSET),
               (ScriptVmState *)state);
    }

    context = gBattleContext;
    func_ov002_020698cc(
        (ScriptVm *)(context + BATTLE_AI_VM_OFFSET),
        (ScriptVmState *)(context + BATTLE_AI_PARTY_STATE_2_OFFSET)
    );

    context = gBattleContext;
    state = (BattleAIState *)(context + BATTLE_AI_PARTY_STATE_3_OFFSET);
    if (state->script != 0) {
        VM_Run((ScriptVm *)(context + BATTLE_AI_VM_OFFSET),
               (ScriptVmState *)state);
    }

    context = gBattleContext;
    state = (BattleAIState *)(context + BATTLE_AI_PARTY_STATE_4_OFFSET);
    if (state->script != 0) {
        VM_Run((ScriptVm *)(context + BATTLE_AI_VM_OFFSET),
               (ScriptVmState *)state);
    }

    BattleTaskList_Update(&gBattleAIActionTaskPool);
    BattleTaskList_Update(&gBattleAIReactionTaskPool);
    BattleTaskList_Update(&gBattleAIAuxTaskPool);
    BattleTaskList_Update(&gBattleAIObjectTaskPool);
}

void BattleAI_StartScriptById(int script_id) {
    int packed_id = script_id;
    int task_type;

    switch (packed_id) {
    case 1:
        BattleAI_StartPartyVmSlot1();
        return;
    case 2:
        BattleAI_StartPartyVmSlot2();
        return;
    case 3:
        BattleAI_StartPartyVmSlot3();
        return;
    case 4:
        BattleAI_StartPartyVmSlot4();
        return;
    }

    task_type = packed_id & BATTLE_AI_TASK_TYPE_MASK;
    switch (task_type) {
    case BATTLE_AI_TASK_ACTION:
        BattleAI_StartActionScript(
            packed_id & BATTLE_AI_TASK_ACTOR_ID_MASK
        );
        return;
    case BATTLE_AI_TASK_REACTION:
        BattleAI_StartReactionScript(
            packed_id & BATTLE_AI_TASK_ACTOR_ID_MASK
        );
        return;
    }
}
