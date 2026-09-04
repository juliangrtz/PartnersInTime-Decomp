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

typedef char ScriptVmCommand_SizeCheck[
    sizeof(ScriptVmCommand) == 0x48 ? 1 : -1
];

s32 VM_ReadVariable(u16 variable, ScriptVm *vm, const u16 **script);
void VM_ReadCommand(ScriptVm *vm, const u16 **script, ScriptVmCommand *command);

#endif
