#ifndef PIT_GAME_SCRIPT_VM_H
#define PIT_GAME_SCRIPT_VM_H

#include <nitro.h>

typedef struct ScriptVm ScriptVm;
typedef struct ScriptVmState ScriptVmState;
typedef struct ScriptVmCommand ScriptVmCommand;
typedef int (*ScriptVmCommandHandler)(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
);

enum ScriptVmCommandDescriptor {
    SCRIPT_VM_ARGUMENT_COUNT_MASK = 0x1F,
    SCRIPT_VM_HAS_RESULT = 0x20,
    SCRIPT_VM_HAS_ARGUMENT_MODES = 0x40
};

enum ScriptVmResult {
    SCRIPT_VM_CONTINUE = 0,
    SCRIPT_VM_FINISHED = 1,
    SCRIPT_VM_RETURNED = 2,
    SCRIPT_VM_YIELDED = 3
};

enum ScriptVmJumpCondition {
    SCRIPT_VM_EQUAL = 0,
    SCRIPT_VM_NOT_EQUAL = 1,
    SCRIPT_VM_LESS_THAN = 2,
    SCRIPT_VM_GREATER_THAN = 3,
    SCRIPT_VM_LESS_OR_EQUAL = 4,
    SCRIPT_VM_GREATER_OR_EQUAL = 5,
    SCRIPT_VM_BITS_SET = 6,
    SCRIPT_VM_EITHER_NONZERO = 7,
    SCRIPT_VM_VALUES_DIFFER = 8,
    SCRIPT_VM_ZERO = 9,
    SCRIPT_VM_NOT_NEGATIVE_ONE = 10
};

enum ScriptVmVariableNamespace {
    SCRIPT_VM_VAR_STATE = 0x1000,
    SCRIPT_VM_VAR_SAVE_FLAGS_48 = 0x2000,
    SCRIPT_VM_VAR_EXTENSION_3 = 0x3000,
    SCRIPT_VM_VAR_EXTENSION_4 = 0x4000,
    SCRIPT_VM_VAR_SAVE_WORDS = 0x5000,
    SCRIPT_VM_VAR_SAVE_BYTES_D0 = 0x6000,
    SCRIPT_VM_VAR_EXTENSION_7 = 0x7000,
    SCRIPT_VM_VAR_CONTEXT = 0x8000,
    SCRIPT_VM_VAR_STATE_24 = 0xA000,
    SCRIPT_VM_VAR_STATE_FLAGS_64 = 0xB000,
    SCRIPT_VM_VAR_SAVE_WORDS_40 = 0xC000,
    SCRIPT_VM_VAR_SAVE_FLAGS_50 = 0xD000,
    SCRIPT_VM_VAR_SAVE_FLAGS_1F0 = 0xE000
};

enum ScriptVmVariableMask {
    SCRIPT_VM_VAR_NAMESPACE_MASK = 0xF000,
    SCRIPT_VM_VAR_INDEX_MASK = 0x0FFF,
    SCRIPT_VM_VAR_BIT_INDEX_MASK = 0x001F,
    SCRIPT_VM_VAR_EXTENDED_NAMESPACE_MASK = 0xE000,
    SCRIPT_VM_VAR_EXTENDED_INDEX_MASK = 0x1FFF
};

struct ScriptVm {
    u8 unknown_00[8];
    ScriptVmCommandHandler command_handler;
    const u32 *command_descriptors;
};

struct ScriptVmCommand {
    u16 opcode;
    u16 result_variable;
    u16 argument_modes;
    u16 padding_06;
    s32 arguments[16];
};

struct ScriptVmState {
    const u16 *script;
    union {
        u8 unknown_04[0x60];
        s32 variables_04[24];
    };
    union {
        s32 loop_stack[17];
        struct {
            s32 loop_stack_base;
            s32 call_stack[16];
        };
    };
    u16 stack_depth;
    u16 delay;
};

typedef char ScriptVmCommand_SizeCheck[
    sizeof(ScriptVmCommand) == 0x48 ? 1 : -1
];
typedef char ScriptVmState_SizeCheck[sizeof(ScriptVmState) == 0xAC ? 1 : -1];

s32 VM_ReadVariable(u16 variable, ScriptVm *vm, ScriptVmState *state);
void VM_WriteVariable(
    u16 variable, s32 value, ScriptVm *vm, ScriptVmState *state
);
int VM_CheckJumpCondition(int condition, s32 left, s32 right);
int VM_ExecuteCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
);
int VM_Run(ScriptVm *vm, ScriptVmState *state);
void VM_ReadCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
);

#endif
