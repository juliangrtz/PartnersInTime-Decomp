#include <game/script_vm.h>

void VM_ReadCommand(ScriptVm *vm, const u16 **script, ScriptVmCommand *command) {
    u16 opcode;
    u16 descriptor;
    u16 argument_count;

    command->opcode = **script;
    opcode = command->opcode;
    (*script)++;
    descriptor = vm->command_descriptors[opcode];

    if (descriptor & SCRIPT_VM_HAS_RESULT) {
        command->result_variable = **script;
        (*script)++;
    }

    argument_count = descriptor & SCRIPT_VM_ARGUMENT_COUNT_MASK;
    if (argument_count == 0) {
        return;
    }

    if (descriptor & SCRIPT_VM_HAS_ARGUMENT_MODES) {
        u16 argument_modes;
        s32 *argument = command->arguments;

        command->argument_modes = **script;
        argument_modes = command->argument_modes;
        (*script)++;

        while (argument_count != 0) {
            s32 value;

            if (argument_modes & 1) {
                value = VM_ReadVariable(**script, vm, script);
            } else {
                value = *(const s16 *)*script;
            }
            *argument = value;
            argument_modes >>= 1;
            (*script)++;
            argument++;
            argument_count--;
        }
    } else {
        s32 *argument = command->arguments;

        while (argument_count != 0) {
            *argument++ = *(const s16 *)*script;
            (*script)++;
            argument_count--;
        }
    }
}
