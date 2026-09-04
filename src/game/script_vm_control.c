#include <game/script_vm.h>

int VM_CheckJumpCondition(int condition, s32 left, s32 right) {
    switch (condition) {
    case SCRIPT_VM_EQUAL:
        return left == right;
    case SCRIPT_VM_NOT_EQUAL:
        return left != right;
    case SCRIPT_VM_LESS_THAN:
        return left < right;
    case SCRIPT_VM_GREATER_THAN:
        return left > right;
    case SCRIPT_VM_LESS_OR_EQUAL:
        return left <= right;
    case SCRIPT_VM_GREATER_OR_EQUAL:
        return left >= right;
    case SCRIPT_VM_BITS_SET:
        return (left & right) != 0;
    case SCRIPT_VM_EITHER_NONZERO:
        return (left | right) != 0;
    case SCRIPT_VM_VALUES_DIFFER:
        return (left ^ right) != 0;
    case SCRIPT_VM_ZERO:
        return left == 0;
    case SCRIPT_VM_NOT_NEGATIVE_ONE:
        return left != -1;
    default:
        return 0;
    }
}

int VM_Run(ScriptVm *vm, ScriptVmState *state) {
    ScriptVmCommand command;
    int result;

    if (state->delay != 0) {
        state->delay--;
        if (state->delay != 0) {
            return SCRIPT_VM_YIELDED;
        }
    }

    do {
        VM_ReadCommand(vm, state, &command);
        result = VM_ExecuteCommand(vm, state, &command);
    } while (result == SCRIPT_VM_CONTINUE);
    return result;
}
