/*
 * Battle VM helpers (overlay 2, 0x02079320-0x02079950).
 *
 * The variable access the battle scripts read and write through, and the target
 * searches they call: highest HP, lowest HP, most damaged, and a random actor
 * eligible for a status.
 */

#include <game/battle_actor.h>
#include <game/battle_ai.h>

extern u32 Random_NextModulo(u32 modulus);

enum BattleVmHelperConstant {
    BATTLE_ACTOR_ACTION_STATE_OFFSET = 0x70,
    BATTLE_ACTOR_REACTION_STATE_OFFSET = 0x128,
    BATTLE_ACTOR_AUXILIARY_STATE_OFFSET = 0x1E0,
    BATTLE_ENEMY_ACTOR_TABLE_OFFSET = 0x6508,
    BATTLE_TARGETABLE_ENEMY_COUNT = 6
};
BattleAIState *BattleAI_GetStateById(int state_id) {
    int task_type;

    switch (state_id) {
    case 1:
        return (BattleAIState *)(gBattleContext
                                 + BATTLE_AI_PARTY_STATE_1_OFFSET);
    case 2:
        return (BattleAIState *)(gBattleContext
                                 + BATTLE_AI_PARTY_STATE_1_OFFSET
                                 + BATTLE_AI_PARTY_STATE_STRIDE);
    case 3:
        return (BattleAIState *)(gBattleContext
                                 + BATTLE_AI_PARTY_STATE_1_OFFSET
                                 + BATTLE_AI_PARTY_STATE_STRIDE * 2);
    case 4:
        return (BattleAIState *)(gBattleContext
                                 + BATTLE_AI_PARTY_STATE_1_OFFSET
                                 + BATTLE_AI_PARTY_STATE_STRIDE * 3);
    }

    task_type = state_id & BATTLE_AI_TASK_TYPE_MASK;
    switch (task_type) {
    case BATTLE_AI_TASK_ACTION:
        return (BattleAIState *)(
            (u8 *)BattleActor_GetEnemySlot(
                (u16)(state_id & BATTLE_AI_TASK_ACTOR_ID_MASK)
            )
            + BATTLE_ACTOR_ACTION_STATE_OFFSET
        );
    case BATTLE_AI_TASK_REACTION:
        return (BattleAIState *)(
            (u8 *)BattleActor_GetEnemySlot(
                (u16)(state_id & BATTLE_AI_TASK_ACTOR_ID_MASK)
            )
            + BATTLE_ACTOR_REACTION_STATE_OFFSET
        );
    case BATTLE_AI_TASK_AUXILIARY:
        return (BattleAIState *)(
            (u8 *)BattleActor_GetEnemySlot(
                (u16)(state_id & BATTLE_AI_TASK_ACTOR_ID_MASK)
            )
            + BATTLE_ACTOR_AUXILIARY_STATE_OFFSET
        );
    case BATTLE_AI_TASK_OBJECT:
        return BattleScriptState_GetByObjectId(
            (u16)(state_id & BATTLE_AI_TASK_ACTOR_ID_MASK)
        );
    default:
        return 0;
    }
}

int BattleActor_FindHighestHpEnemy(int minimum_current_hp) {
    int selected_actor_id;
    int slot;
    BattleActor **actors;

    selected_actor_id = -1;
    actors = (BattleActor **)(gBattleContext
                              + BATTLE_ENEMY_ACTOR_TABLE_OFFSET);
    slot = 0;

    do {
        BattleActor *actor = *actors;
        if (actor != 0 && actor->resource_slot != 0
            && actor->current_hp >= minimum_current_hp) {
            minimum_current_hp = (u16)actor->current_hp;
            selected_actor_id = (s16)(slot + BATTLE_ACTOR_ENEMY_FIRST);
        }
        slot = (s16)(slot + 1);
        actors++;
    } while (slot < BATTLE_TARGETABLE_ENEMY_COUNT);

    return selected_actor_id;
}

int BattleActor_FindLowestHpEnemy(int maximum_current_hp) {
    int selected_actor_id;
    int slot;
    BattleActor **actors;

    selected_actor_id = -1;
    actors = (BattleActor **)(gBattleContext
                              + BATTLE_ENEMY_ACTOR_TABLE_OFFSET);
    slot = 0;

    do {
        BattleActor *actor = *actors;
        if (actor != 0 && actor->resource_slot != 0
            && actor->current_hp <= maximum_current_hp) {
            maximum_current_hp = (u16)actor->current_hp;
            selected_actor_id = (s16)(slot + BATTLE_ACTOR_ENEMY_FIRST);
        }
        slot = (s16)(slot + 1);
        actors++;
    } while (slot < BATTLE_TARGETABLE_ENEMY_COUNT);

    return selected_actor_id;
}

int BattleActor_FindMostDamagedEnemy(int minimum_current_hp) {
    int selected_actor_id;
    int slot;
    int largest_missing_hp;
    BattleActor **actors;
    BattleActor *actor;
    int current_hp;
    int missing_hp;

    slot = 0;
    largest_missing_hp = 0;
    actors = (BattleActor **)(gBattleContext
                              + BATTLE_ENEMY_ACTOR_TABLE_OFFSET);
    selected_actor_id = -1;

    do {
        actor = *actors;
        if (actor != 0) {
            if (actor->resource_slot != 0) {
                current_hp = actor->current_hp;
                if (current_hp >= minimum_current_hp) {
                    missing_hp = actor->max_hp - current_hp;
                    if (missing_hp >= largest_missing_hp) {
                        largest_missing_hp = (s16)missing_hp;
                        selected_actor_id =
                            (s16)(slot + BATTLE_ACTOR_ENEMY_FIRST);
                    }
                }
            }
        }
        slot = (s16)(slot + 1);
        actors++;
    } while (slot < BATTLE_TARGETABLE_ENEMY_COUNT);

    return selected_actor_id;
}

int BattleActor_SelectRandomStatusTarget(u16 first_actor_id, u32 actor_count) {
    u16 actor_id = first_actor_id;
    u16 selected_actor_id = 0;
    int eligible_count = 0;

    actor_count = (u16)(actor_count + first_actor_id);
    if (actor_id < actor_count) {
        do {
            if (actor_id != 58 && actor_id != 59) {
                BattleActor *actor = BattleActor_GetById(actor_id);
                if (BattleActor_CanReceiveStatus(actor)) {
                    eligible_count++;
                    if (Random_NextModulo(eligible_count) == 0) {
                        selected_actor_id = actor_id;
                    }
                }
            }
            actor_id = (u16)(actor_id + 1);
        } while (actor_id < actor_count);
    }

    return selected_actor_id;
}

enum BattleContextVariableOffset {
    BATTLE_CONTEXT_VARIABLE_18_OFFSET = 0x18,
    BATTLE_CONTEXT_VARIABLE_20_OFFSET = 0x20,
    BATTLE_CONTEXT_MASK_102_OFFSET = 0x102,
    BATTLE_CONTEXT_MASK_104_OFFSET = 0x104,
    BATTLE_CONTEXT_MASK_106_OFFSET = 0x106,
    BATTLE_CONTEXT_MASK_EXCLUDE_OFFSET = 0x110,
    BATTLE_CONTEXT_SHARED_VARIABLES_OFFSET = 0x69E4
};
s32 BattleVM_ReadVariable(
    u16 variable, ScriptVm *vm, ScriptVmState *state, u32 index
) {
    BattleAIState *battle_state = (BattleAIState *)state;

    switch (variable) {
    case BATTLE_VM_VAR_OWNER_ACTOR_ID:
        return battle_state->owner_id & BATTLE_AI_TASK_ACTOR_ID_MASK;
    case BATTLE_VM_VAR_OWNER_TASK_TYPE:
        return battle_state->owner_id >> 12;
    case BATTLE_VM_VAR_CONSTANT_ZERO_2:
        return 0;
    case BATTLE_VM_VAR_CONSTANT_ZERO_3:
        return 0;
    case BATTLE_VM_VAR_CONTEXT_18:
        return *(s32 *)(gBattleContext + BATTLE_CONTEXT_VARIABLE_18_OFFSET);
    case BATTLE_VM_VAR_TARGET_ACTOR_ID:
        return (u16)BattleActor_GetById(
                   (u16)(battle_state->owner_id
                         & BATTLE_AI_TASK_ACTOR_ID_MASK)
               )
            ->target_actor_id;
    case BATTLE_VM_VAR_CONTEXT_MASK_102:
        return *(u16 *)(gBattleContext + BATTLE_CONTEXT_MASK_102_OFFSET)
            & ~*(u16 *)(gBattleContext + BATTLE_CONTEXT_MASK_EXCLUDE_OFFSET);
    case BATTLE_VM_VAR_CONTEXT_MASK_104:
        return *(u16 *)(gBattleContext + BATTLE_CONTEXT_MASK_104_OFFSET)
            & ~*(u16 *)(gBattleContext + BATTLE_CONTEXT_MASK_EXCLUDE_OFFSET);
    case BATTLE_VM_VAR_CONTEXT_MASK_106:
        return *(u16 *)(gBattleContext + BATTLE_CONTEXT_MASK_106_OFFSET)
            & ~*(u16 *)(gBattleContext + BATTLE_CONTEXT_MASK_EXCLUDE_OFFSET);
    case BATTLE_VM_VAR_CONTEXT_20:
        return *(u16 *)(gBattleContext + BATTLE_CONTEXT_VARIABLE_20_OFFSET);
    default:
        if (variable >= BATTLE_VM_VAR_SHARED_FIRST
            && variable <= BATTLE_VM_VAR_SHARED_LAST) {
            return *(s32 *)(gBattleContext
                           + BATTLE_CONTEXT_SHARED_VARIABLES_OFFSET
                           + (variable - BATTLE_VM_VAR_SHARED_FIRST)
                               * sizeof(s32));
        }
        return 0;
    }
}

void BattleVM_WriteVariable(
    u16 variable, s32 value, ScriptVm *vm, ScriptVmState *state
) {
    BattleAIState *battle_state = (BattleAIState *)state;

    switch (variable) {
    case BATTLE_VM_VAR_TARGET_ACTOR_ID:
        BattleActor_GetById(
            (u16)(battle_state->owner_id & BATTLE_AI_TASK_ACTOR_ID_MASK)
        )
            ->target_actor_id = value;
        break;
    case BATTLE_VM_VAR_CONTEXT_18:
        *(s32 *)(gBattleContext + BATTLE_CONTEXT_VARIABLE_18_OFFSET) = value;
        break;
    default:
        if (variable >= BATTLE_VM_VAR_SHARED_FIRST
            && variable <= BATTLE_VM_VAR_SHARED_LAST) {
            *(s32 *)(gBattleContext + BATTLE_CONTEXT_SHARED_VARIABLES_OFFSET
                    + (variable - BATTLE_VM_VAR_SHARED_FIRST) * sizeof(s32)) =
                value;
        }
        break;
    }
}
