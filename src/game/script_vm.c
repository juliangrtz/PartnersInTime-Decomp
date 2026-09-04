#include <game/script_vm.h>

void VM_ReadCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
) {
    u16 opcode;
    u16 descriptor;
    u16 argument_count;

    command->opcode = *state->script;
    opcode = command->opcode;
    state->script++;
    descriptor = vm->command_descriptors[opcode];

    if (descriptor & SCRIPT_VM_HAS_RESULT) {
        command->result_variable = *state->script;
        state->script++;
    }

    argument_count = descriptor & SCRIPT_VM_ARGUMENT_COUNT_MASK;
    if (argument_count == 0) {
        return;
    }

    if (descriptor & SCRIPT_VM_HAS_ARGUMENT_MODES) {
        u16 argument_modes;
        s32 *argument = command->arguments;

        command->argument_modes = *state->script;
        argument_modes = command->argument_modes;
        state->script++;

        while (argument_count != 0) {
            s32 value;

            if (argument_modes & 1) {
                value = VM_ReadVariable(*state->script, vm, state);
            } else {
                value = *(const s16 *)state->script;
            }
            *argument = value;
            argument_modes >>= 1;
            state->script++;
            argument++;
            argument_count--;
        }
    } else {
        s32 *argument = command->arguments;

        while (argument_count != 0) {
            *argument++ = *(const s16 *)state->script;
            state->script++;
            argument_count--;
        }
    }
}
