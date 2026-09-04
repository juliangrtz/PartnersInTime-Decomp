#include <game/battle_ai.h>

extern void BattleAI_StartScriptTask(u16 actor_id, int task_type);

enum BattleAIPartyStateOffset {
    BATTLE_AI_PARTY_SLOT_1_STATE_OFFSET = 0x6A64,
    BATTLE_AI_PARTY_SLOT_3_STATE_OFFSET = 0x6BD4,
    BATTLE_AI_PARTY_SLOT_4_STATE_OFFSET = 0x6C8C,
    BATTLE_AI_PARTY_SLOT_1_SCRIPT_OFFSET = 0xD3BC,
    BATTLE_AI_PARTY_SLOT_3_SCRIPT_OFFSET = 0xD3C4,
    BATTLE_AI_PARTY_SLOT_4_SCRIPT_OFFSET = 0x35608
};

void BattleAI_StartReactionScript(int actor_id) {
    BattleAI_StartScriptTask((u16)actor_id, BATTLE_AI_TASK_REACTION);
}

void BattleAI_StartActionScript(int actor_id) {
    BattleAI_StartScriptTask((u16)actor_id, BATTLE_AI_TASK_ACTION);
}

void BattleAI_StartPartyVmSlot4(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext +
                          BATTLE_AI_PARTY_SLOT_4_STATE_OFFSET),
        (const u16 *)(gBattleContext +
                      BATTLE_AI_PARTY_SLOT_4_SCRIPT_OFFSET),
        4);
}

void BattleAI_StartPartyVmSlot3(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext +
                          BATTLE_AI_PARTY_SLOT_3_STATE_OFFSET),
        *(const u16 **)(gBattleContext +
                        BATTLE_AI_PARTY_SLOT_3_SCRIPT_OFFSET),
        3);
}

void BattleAI_StartPartyVmSlot2(void) {
}

void BattleAI_StartPartyVmSlot1(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext +
                          BATTLE_AI_PARTY_SLOT_1_STATE_OFFSET),
        *(const u16 **)(gBattleContext +
                        BATTLE_AI_PARTY_SLOT_1_SCRIPT_OFFSET),
        1);
}
