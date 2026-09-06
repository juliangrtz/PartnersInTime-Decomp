#include <game/battle_actor.h>
#include <game/battle_ai.h>

extern void func_0202cbd4(void *destination, int value, u32 size);

enum BattleAIPartyStateOffset {
    BATTLE_AI_PARTY_SLOT_1_SCRIPT_OFFSET = 0xD3BC,
    BATTLE_AI_PARTY_SLOT_3_SCRIPT_OFFSET = 0xD3C4,
    BATTLE_AI_PARTY_SLOT_4_SCRIPT_OFFSET = 0x35608
};

enum BattleAIActorTableOffset {
    BATTLE_AI_ENEMY_ACTOR_TABLE_OFFSET = 0x6508
};

typedef struct BattleAIScriptResource {
    u32 unknown_00;
    const void *script;
} BattleAIScriptResource;

typedef struct BattleAIEnemyActor {
    BattleActor actor;
    u8 action_state[0xB8];
    u8 reaction_state[0xB8];
    BattleAIState auxiliary_state;
} BattleAIEnemyActor;

void BattleAI_InitStateFromScriptBlock(BattleAIState *state,
                                       const u16 *script_block, int owner_id) {
    func_0202cbd4(state, 0, 0xB8);
    state->script = script_block + ((u32)*script_block >> 1);
    state->owner_id = owner_id;
}

void BattleAI_StartScriptTask(u16 actor_id, int task_type) {
    BattleAIEnemyActor *actor =
        *(BattleAIEnemyActor **)(gBattleContext +
                                 BATTLE_AI_ENEMY_ACTOR_TABLE_OFFSET +
                                 (s16)(actor_id - BATTLE_ACTOR_ENEMY_FIRST) * 4);
    BattleAIScriptResource *resource =
        (BattleAIScriptResource *)actor->actor.resource_slot;
    BattleAIState *state;
    void (*callback)(BattleAITask *task);
    BattleTaskPool *pool;
    BattleAITask *task;

    if (task_type == BATTLE_AI_TASK_ACTION) {
        state = (BattleAIState *)actor->action_state;
        pool = &gBattleAIActionTaskPool;
        callback = BattleAI_UpdateActionTask;
    } else if (task_type == BATTLE_AI_TASK_REACTION) {
        pool = &gBattleAIReactionTaskPool;
        callback = BattleAI_UpdateReactionTask;
        state = (BattleAIState *)actor->reaction_state;
    }

    func_0202cbd4(state, 0, 0xB8);
    state->script = resource->script;
    state->owner_id = task_type | actor_id;

    task = BattleAITask_GetOrInsert(pool, actor_id);
    task->callback = callback;
    task->state = state;
    task->id = actor_id;
}

void BattleAI_StartPartyVmSlot1(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext +
                          BATTLE_AI_PARTY_STATE_1_OFFSET),
        *(const u16 **)(gBattleContext +
                        BATTLE_AI_PARTY_SLOT_1_SCRIPT_OFFSET),
        1);
}

void BattleAI_StartPartyVmSlot2(void) {
}

void BattleAI_StartPartyVmSlot3(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext +
                          BATTLE_AI_PARTY_STATE_3_OFFSET),
        *(const u16 **)(gBattleContext +
                        BATTLE_AI_PARTY_SLOT_3_SCRIPT_OFFSET),
        3);
}

void BattleAI_StartPartyVmSlot4(void) {
    BattleAI_InitStateFromScriptBlock(
        (BattleAIState *)(gBattleContext +
                          BATTLE_AI_PARTY_STATE_4_OFFSET),
        (const u16 *)(gBattleContext +
                      BATTLE_AI_PARTY_SLOT_4_SCRIPT_OFFSET),
        4);
}

void BattleAI_StartActionScript(int actor_id) {
    BattleAI_StartScriptTask((u16)actor_id, BATTLE_AI_TASK_ACTION);
}

void BattleAI_StartReactionScript(int actor_id) {
    BattleAI_StartScriptTask((u16)actor_id, BATTLE_AI_TASK_REACTION);
}

void BattleAI_StartActorAuxScript(u16 actor_id, int target_actor_id,
                                  const void *script) {
    BattleAIEnemyActor *actor =
        *(BattleAIEnemyActor **)(gBattleContext +
                                 BATTLE_AI_ENEMY_ACTOR_TABLE_OFFSET +
                                 (s16)(actor_id - BATTLE_ACTOR_ENEMY_FIRST) * 4);
    BattleAIState *state = &actor->auxiliary_state;
    BattleAITask *task;

    actor->actor.target_actor_id = target_actor_id;
    state->script = script;
    state->scratch_a8 = 0;
    state->scratch_aa = 0;
    state->disabled = 0;
    state->order = 0;
    state->owner_id = BATTLE_AI_TASK_AUXILIARY | actor_id;

    task = BattleAITask_GetOrInsert(&gBattleAIAuxTaskPool, actor_id);
    task->callback = BattleAI_UpdateAuxTask;
    task->state = state;
    task->id = actor_id;
}

void BattleAI_StartObjectScript(int object_id, const void *script,
                                BattleAIState *source_state,
                                int queue_continuation) {
    BattleAIState *state = BattleScriptState_GetByObjectId((u16)object_id);
    BattleAITask *task;

    if (state->script != 0 && queue_continuation) {
        if (source_state->owner_id < BATTLE_AI_TASK_OBJECT) {
            state->continuation_order = source_state->owner_id;
            state->continuation_tie_break = 0xFFFF;
        } else {
            state->continuation_order = source_state->order_value;
            state->continuation_tie_break =
                source_state->owner_id & BATTLE_AI_TASK_ACTOR_ID_MASK;
        }
        state->continuation_script = script;
    } else {
        if (source_state->owner_id < BATTLE_AI_TASK_OBJECT) {
            state->order = source_state->owner_id;
            state->order_tie_break = 0xFFFF;
        } else {
            state->order = source_state->order_value;
            state->order_tie_break =
                source_state->owner_id & BATTLE_AI_TASK_ACTOR_ID_MASK;
        }
        state->script = script;
        state->scratch_a8 = 0;
        state->scratch_aa = 0;
        state->disabled = 0;
        state->continuation_script = 0;
        state->continuation_order = 0;
        state->continuation_tie_break = 0xFFFF;
    }

    state->owner_id = BATTLE_AI_TASK_OBJECT | object_id;
    task = BattleAITask_GetOrInsert(&gBattleAIObjectTaskPool, object_id);
    task->callback = BattleAI_UpdateChainedTask;
    task->state = state;
    task->id = object_id;
}
