#ifndef PIT_GAME_BATTLE_AI_H
#define PIT_GAME_BATTLE_AI_H

#include <nitro.h>

typedef struct BattleAIState BattleAIState;
typedef struct BattleAITask BattleAITask;
typedef struct BattleTaskPool BattleTaskPool;

enum BattleAIVmResult {
    BATTLE_AI_VM_FINISHED = 1,
    BATTLE_AI_VM_CHAIN_SCRIPT = 2,
    BATTLE_AI_VM_YIELD = 3
};

enum BattleAIScriptTaskType {
    BATTLE_AI_TASK_ACTION = 0x1000,
    BATTLE_AI_TASK_REACTION = 0x2000
};

extern u8 *gBattleContext;

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

struct BattleTaskPool {
    BattleAITask *active;
    BattleAITask *free;
};

typedef char BattleAIState_SizeCheck[sizeof(BattleAIState) == 0xC0 ? 1 : -1];
typedef char BattleAITask_SizeCheck[sizeof(BattleAITask) == 0x14 ? 1 : -1];
typedef char BattleTaskPool_SizeCheck[sizeof(BattleTaskPool) == 8 ? 1 : -1];

int BattleAI_HandleVmResult(BattleAITask *task, int result, BattleAIState *state);
void BattleAI_StartReactionScript(int actor_id);
void BattleAI_StartActionScript(int actor_id);
void BattleAI_StartPartyVmSlot4(void);
void BattleAI_StartPartyVmSlot3(void);
void BattleAI_StartPartyVmSlot2(void);
void BattleAI_StartPartyVmSlot1(void);
void BattleAI_InitStateFromScriptBlock(BattleAIState *state,
                                       const u16 *script_block, int owner_id);
BattleAIState *BattleScriptState_GetByObjectId(u16 object_id);

void BattlePool_ReturnNode(BattleTaskPool *pool, BattleAITask *task);
BattleAITask *BattlePool_TakeNode(BattleTaskPool *pool);
BattleAITask *BattleTask_BindOwnerSlot(BattleAITask *task,
                                      BattleAITask **owner_slot);
void BattleTask_Release(BattleAITask *task);
BattleAITask *BattleTaskList_Insert(BattleTaskPool *pool, BattleAITask *task);
BattleAITask *BattleTaskPool_Allocate(BattleTaskPool *pool);
void BattleTaskPool_Init(BattleTaskPool *pool, int count, u32 payload_size);

#endif
