#include <game/battle_ai.h>

extern void BattleAI_StartScriptTask(u16 actor_id, int task_type);

void BattleAI_StartReactionScript(int actor_id) {
    BattleAI_StartScriptTask((u16)actor_id, 0x2000);
}

void BattleAI_StartActionScript(int actor_id) {
    BattleAI_StartScriptTask((u16)actor_id, 0x1000);
}
