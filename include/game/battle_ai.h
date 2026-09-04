#ifndef PIT_GAME_BATTLE_AI_H
#define PIT_GAME_BATTLE_AI_H

#include <nitro.h>

typedef struct BattleAIState BattleAIState;
typedef struct BattleAITask BattleAITask;

struct BattleAIState {
    const void *script;
    u8 unk_004[0xA4];
    u16 scratch_a8;
    u16 scratch_aa;
    u8 unk_0ac[4];
    u16 owner_id;
    u16 flags;
    s16 order;
    u16 order_tie_break;
    const void *continuation_script;
    u16 continuation_order;
    u16 continuation_tie_break;
};

struct BattleAITask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleAIState *state;
    u16 id;
    u16 padding_12;
};

typedef char BattleAIState_SizeCheck[sizeof(BattleAIState) == 0xC0 ? 1 : -1];
typedef char BattleAITask_SizeCheck[sizeof(BattleAITask) == 0x14 ? 1 : -1];

int BattleAI_HandleVmResult(BattleAITask *task, int result, BattleAIState *state);
void BattleAI_StartPartyVmSlot2(void);
BattleAIState *BattleScriptState_GetByObjectId(u16 object_id);

#endif
