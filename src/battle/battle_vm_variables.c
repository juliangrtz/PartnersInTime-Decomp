#include <game/battle_actor.h>
#include <game/battle_ai.h>

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
        return battle_state->owner_id & 0xFFF;
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
                   (u16)(battle_state->owner_id & 0xFFF)
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
        BattleActor_GetById((u16)(battle_state->owner_id & 0xFFF))
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
