#include <game/battle_ai.h>
#include <game/script_vm.h>

extern u8 *gSaveData;

extern s32 func_ov000_02081c00(
    ScriptVm *vm, ScriptVmState *state, u16 variable, u32 index
);
extern void func_ov007_020841f8(
    u16 variable, s32 value, ScriptVm *vm, ScriptVmState *state
);
extern s32 func_ov007_02084240(
    u16 variable, ScriptVm *vm, ScriptVmState *state, u32 index
);

static inline s32 *VM_ContextVariables(ScriptVm *vm) {
    return (s32 *)((u8 *)vm + 0x10);
}

s32 VM_ReadVariable(u16 variable, ScriptVm *vm, ScriptVmState *state) {
    u16 namespace_id;
    u32 index;

    if ((variable & SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK)
        == SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK) {
        index = variable & ~SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK;
        return (
            ((u32 *)(gSaveData + 0x1F0))[index >> 5]
            >> (index & SCRIPT_VM_VAR_BIT_INDEX_MASK)
        ) & 1;
    }

    namespace_id = variable & SCRIPT_VM_VAR_NAMESPACE_MASK;
    index = variable & SCRIPT_VM_VAR_INDEX_MASK;
    switch (namespace_id) {
    case SCRIPT_VM_VAR_SAVE_FLAGS_48:
        return (
            ((u32 *)(gSaveData + 0x48))[index >> 5]
            >> (index & SCRIPT_VM_VAR_BIT_INDEX_MASK)
        ) & 1;
    case SCRIPT_VM_VAR_SAVE_FLAGS_50:
        return (
            ((u32 *)(gSaveData + 0x50))[index >> 5]
            >> (index & SCRIPT_VM_VAR_BIT_INDEX_MASK)
        ) & 1;
    case SCRIPT_VM_VAR_EXTENSION_3:
        return func_ov000_02081c00(vm, state, variable, index);
    case SCRIPT_VM_VAR_EXTENSION_4:
        return BattleVM_ReadVariable(variable, vm, state, index);
    case SCRIPT_VM_VAR_EXTENSION_7:
        return func_ov007_02084240(variable, vm, state, index);
    case SCRIPT_VM_VAR_SAVE_WORDS:
        return ((s32 *)gSaveData)[index];
    case SCRIPT_VM_VAR_SAVE_BYTES_D0: {
        u8 *save = gSaveData + index;
        return save[0xD0];
    }
    case SCRIPT_VM_VAR_CONTEXT:
        return VM_ContextVariables(vm)[index];
    case SCRIPT_VM_VAR_STATE_24:
        return *(s32 *)((u8 *)state + 0x24 + index * 4);
    case SCRIPT_VM_VAR_STATE_FLAGS_64:
        return (
            ((u32 *)state->loop_stack)[index >> 5]
            >> (index & SCRIPT_VM_VAR_BIT_INDEX_MASK)
        ) & 1;
    case SCRIPT_VM_VAR_SAVE_WORDS_40:
        return *(s32 *)(gSaveData + 0x40 + index * 4);
    case SCRIPT_VM_VAR_STATE:
        return state->variables_04[index];
    default:
        return 0;
    }
}

void VM_WriteVariable(
    u16 variable, s32 value, ScriptVm *vm, ScriptVmState *state
) {
    u16 namespace_id;
    u32 index;

    if ((variable & SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK)
        == SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK) {
        index = variable & ~SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK;
        if (value != 0) {
            u32 *words = (u32 *)(gSaveData + 0x1F0);
            words[index >> 5] |= 1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK);
        } else {
            u32 *words = (u32 *)(gSaveData + 0x1F0);
            words[index >> 5] &= ~(1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK));
        }
        return;
    }

    namespace_id = variable & SCRIPT_VM_VAR_NAMESPACE_MASK;
    index = variable & ~SCRIPT_VM_VAR_NAMESPACE_MASK;
    switch (namespace_id) {
    case SCRIPT_VM_VAR_STATE:
        state->variables_04[index] = value;
        return;
    case SCRIPT_VM_VAR_SAVE_FLAGS_48:
        if (value != 0) {
            u32 *words = (u32 *)(gSaveData + 0x48);
            words[index >> 5] |= 1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK);
        } else {
            u32 *words = (u32 *)(gSaveData + 0x48);
            words[index >> 5] &= ~(1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK));
        }
        return;
    case SCRIPT_VM_VAR_SAVE_FLAGS_50:
        if (value != 0) {
            u32 *words = (u32 *)(gSaveData + 0x50);
            words[index >> 5] |= 1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK);
        } else {
            u32 *words = (u32 *)(gSaveData + 0x50);
            words[index >> 5] &= ~(1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK));
        }
        return;
    case SCRIPT_VM_VAR_EXTENSION_3:
    case SCRIPT_VM_VAR_EXTENSION_4:
        BattleVM_WriteVariable(variable, value, vm, state);
        return;
    case SCRIPT_VM_VAR_SAVE_WORDS:
        ((s32 *)gSaveData)[index] = value;
        return;
    case SCRIPT_VM_VAR_EXTENSION_7:
        func_ov007_020841f8(variable, value, vm, state);
        return;
    case SCRIPT_VM_VAR_SAVE_BYTES_D0: {
        u8 *save = gSaveData + index;
        save[0xD0] = value;
        return;
    }
    case SCRIPT_VM_VAR_CONTEXT:
        VM_ContextVariables(vm)[index] = value;
        return;
    case SCRIPT_VM_VAR_STATE_24:
        *(s32 *)((u8 *)state + 0x24 + index * 4) = value;
        return;
    case SCRIPT_VM_VAR_STATE_FLAGS_64:
        if (value != 0) {
            ((u32 *)state->loop_stack)[index >> 5]
                |= 1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK);
        } else {
            ((u32 *)state->loop_stack)[index >> 5]
                &= ~(1 << (index & SCRIPT_VM_VAR_BIT_INDEX_MASK));
        }
        return;
    case SCRIPT_VM_VAR_SAVE_WORDS_40:
        *(s32 *)(gSaveData + 0x40 + index * 4) = value;
        return;
    }
}
