#ifndef PIT_GAME_SCRIPT_VM_H
#define PIT_GAME_SCRIPT_VM_H

#include <nitro.h>

typedef struct ScriptVm ScriptVm;
typedef struct ScriptVmCommand ScriptVmCommand;

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

struct ScriptVm {
    u8 unknown_00[0x0C];
    const u32 *command_descriptors;
};

struct ScriptVmCommand {
    u16 opcode;
    u16 result_variable;
    u16 argument_modes;
    u16 padding_06;
    s32 arguments[16];
};

typedef struct ScriptVmState {
    const u16 *script;
    u8 unknown_04[0x60];
    s32 stack[17];
    u16 stack_depth;
    u16 delay;
} ScriptVmState;

typedef char ScriptVmCommand_SizeCheck[
    sizeof(ScriptVmCommand) == 0x48 ? 1 : -1
];
typedef char ScriptVmState_SizeCheck[sizeof(ScriptVmState) == 0xAC ? 1 : -1];

s32 VM_ReadVariable(u16 variable, ScriptVm *vm, const u16 **script);
int VM_CheckJumpCondition(int condition, s32 left, s32 right);
int VM_ExecuteCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
);
int VM_Run(ScriptVm *vm, ScriptVmState *state);
void VM_ReadCommand(ScriptVm *vm, const u16 **script, ScriptVmCommand *command);

#endif
