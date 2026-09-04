#ifndef PIT_GAME_BATTLE_AI_H
#define PIT_GAME_BATTLE_AI_H

#include <game/script_vm.h>
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
    BATTLE_AI_TASK_REACTION = 0x2000,
    BATTLE_AI_TASK_AUXILIARY = 0x3000,
    BATTLE_AI_TASK_OBJECT = 0x4000,
    BATTLE_AI_TASK_TYPE_MASK = 0xF000,
    BATTLE_AI_TASK_ACTOR_ID_MASK = 0x0FFF
};

enum BattleAIStateFlag {
    BATTLE_AI_STATE_FLAG_DISABLED = 1 << 0,
    BATTLE_AI_STATE_FLAG_ORDER_WAIT = 1 << 1
};

enum BattleAIConstant {
    BATTLE_AI_ARCHIVE_COUNT = 14
};

enum BattleAIContextOffset {
    BATTLE_AI_VM_OFFSET = 0x6954,
    BATTLE_AI_PARTY_STATE_1_OFFSET = 0x6A64,
    BATTLE_AI_PARTY_STATE_2_OFFSET = 0x6B1C,
    BATTLE_AI_PARTY_STATE_3_OFFSET = 0x6BD4,
    BATTLE_AI_PARTY_STATE_4_OFFSET = 0x6C8C,
    BATTLE_AI_PARTY_STATE_STRIDE = 0xB8
};

enum BattleVmVariable {
    BATTLE_VM_VAR_OWNER_ACTOR_ID = 0x4000,
    BATTLE_VM_VAR_OWNER_TASK_TYPE = 0x4001,
    BATTLE_VM_VAR_CONSTANT_ZERO_2 = 0x4002,
    BATTLE_VM_VAR_CONSTANT_ZERO_3 = 0x4003,
    BATTLE_VM_VAR_CONTEXT_18 = 0x4005,
    BATTLE_VM_VAR_TARGET_ACTOR_ID = 0x4006,
    BATTLE_VM_VAR_CONTEXT_MASK_102 = 0x4007,
    BATTLE_VM_VAR_CONTEXT_MASK_104 = 0x4008,
    BATTLE_VM_VAR_CONTEXT_MASK_106 = 0x4009,
    BATTLE_VM_VAR_CONTEXT_20 = 0x400A,
    BATTLE_VM_VAR_SHARED_FIRST = 0x4010,
    BATTLE_VM_VAR_SHARED_LAST = 0x402F
};

extern u8 *gBattleContext;
extern BattleTaskPool gBattleAIAuxTaskPool;
extern BattleTaskPool gBattleAIActionTaskPool;
extern BattleTaskPool gBattleAIReactionTaskPool;
extern BattleTaskPool gBattleAIObjectTaskPool;

struct BattleAIState {
    const void *script;
    u8 unk_004[0xA4];
    u16 scratch_a8;
    u16 scratch_aa;
    u8 unk_0ac[4];
    u16 owner_id;
    union {
        u16 flags;
        struct {
            u16 disabled : 1;
            u16 wait_for_order : 1;
            u16 unknown_flags : 14;
        };
    };
    union {
        s16 order;
        u16 order_value;
    };
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
void BattleAI_UpdateChainedTask(BattleAITask *task);
void BattleAI_UpdateAuxTask(BattleAITask *task);
void BattleAI_UpdateReactionTask(BattleAITask *task);
void BattleAI_UpdateActionTask(BattleAITask *task);
BattleAITask *BattleAITask_GetOrInsert(BattleTaskPool *pool, int actor_id);
void BattleAITask_StopById(BattleAITask **head, int actor_id);
void BattleAI_StopScriptById(int script_id);
int BattleAI_TryClearOrderWait(BattleAIState *state);
void BattleAI_StartObjectScript(int object_id, const void *script,
                                BattleAIState *source_state,
                                int queue_continuation);
void BattleAI_StartActorAuxScript(u16 actor_id, u16 target_actor_id,
                                  const void *script);
void BattleAI_StartScriptTask(u16 actor_id, int task_type);
void BattleAI_StartReactionScript(int actor_id);
void BattleAI_StartActionScript(int actor_id);
void BattleAI_StartPartyVmSlot4(void);
void BattleAI_StartPartyVmSlot3(void);
void BattleAI_StartPartyVmSlot2(void);
void BattleAI_StartPartyVmSlot1(void);
void BattleAI_StartScriptById(int script_id);
void BattleAI_UpdateAll(void);
void BattleAI_TaskPoolsInit(void);
void BattleAI_Initialize(void);
int BattleAI_DispatchOpcode(ScriptVm *vm, ScriptVmState *state,
                            ScriptVmCommand *command);
int BattleActor_IsHitLocked(int actor_id);
void BattleAI_UpdateControlMask(int mask, int enabled);
void BattleAI_InitStateFromScriptBlock(BattleAIState *state,
                                       const u16 *script_block, int owner_id);
BattleAIState *BattleScriptState_GetByObjectId(u16 object_id);
BattleAIState *BattleAI_GetStateById(int state_id);

int BattleActor_SelectRandomStatusTarget(u16 first_actor_id, u32 actor_count);
int BattleActor_FindMostDamagedEnemy(int minimum_current_hp);
int BattleActor_FindLowestHpEnemy(int maximum_current_hp);
int BattleActor_FindHighestHpEnemy(int minimum_current_hp);

s32 BattleVM_ReadVariable(
    u16 variable, ScriptVm *vm, ScriptVmState *state, u32 index
);
void BattleVM_WriteVariable(
    u16 variable, s32 value, ScriptVm *vm, ScriptVmState *state
);

void BattlePool_ReturnNode(BattleTaskPool *pool, BattleAITask *task);
BattleAITask *BattlePool_TakeNode(BattleTaskPool *pool);
BattleAITask *BattleTask_BindOwnerSlot(BattleAITask *task,
                                      BattleAITask **owner_slot);
void BattleTask_Release(BattleAITask *task);
BattleAITask *BattleTaskList_Insert(BattleTaskPool *pool, BattleAITask *task);
BattleAITask *BattleTaskPool_Allocate(BattleTaskPool *pool);
void BattleTaskPool_Init(BattleTaskPool *pool, int count, u32 payload_size);
int BattleTaskList_Update(BattleTaskPool *pool);

#endif
