#include <game/battle_ai.h>

void BattleAI_StartPartyVmSlot4(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext + 0x6C8C),
        (const u16 *)(gBattleContext + 0x35608), 4);
}

void BattleAI_StartPartyVmSlot3(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext + 0x6BD4),
        *(const u16 **)(gBattleContext + 0xD3C4), 3);
}

void BattleAI_StartPartyVmSlot2(void) {
}

void BattleAI_StartPartyVmSlot1(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext + 0x6A64),
        *(const u16 **)(gBattleContext + 0xD3BC), 1);
}
