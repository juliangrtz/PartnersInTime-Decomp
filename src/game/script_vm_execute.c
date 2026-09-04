#include <game/script_vm.h>
#include <nitro/fx.h>

extern int FX_AtanIdx(fx32 tangent);
extern int FX_Atan2Idx(fx32 y, fx32 x);
extern s16 FX_SinCosTable_[8192];
extern u32 Random_NextModulo(u32 modulus);
extern void (*data_02059d8c)(void *marker);
extern u32 data_0205003c;
extern u32 data_02050040;

enum ScriptVmCoreOpcode {
    SCRIPT_VM_OP_END = 0,
    SCRIPT_VM_OP_RETURN = 1,
    SCRIPT_VM_OP_JUMP = 2,
    SCRIPT_VM_OP_WAIT = 3,
    SCRIPT_VM_OP_JUMP_IF = 4,
    SCRIPT_VM_OP_UNKNOWN_05 = 5,
    SCRIPT_VM_OP_UNKNOWN_06 = 6,
    SCRIPT_VM_OP_UNKNOWN_07 = 7,
    SCRIPT_VM_OP_UNKNOWN_08 = 8,
    SCRIPT_VM_OP_UNKNOWN_09 = 9,
    SCRIPT_VM_OP_PUSH = 10,
    SCRIPT_VM_OP_POP = 11,
    SCRIPT_VM_OP_LOOP = 12,
    SCRIPT_VM_OP_READ_TABLE = 13,
    SCRIPT_VM_OP_READ_SCRIPT = 14,
    SCRIPT_VM_OP_SET = 15,
    SCRIPT_VM_OP_ADD = 16,
    SCRIPT_VM_OP_SUBTRACT = 17,
    SCRIPT_VM_OP_MULTIPLY = 18,
    SCRIPT_VM_OP_DIVIDE = 19,
    SCRIPT_VM_OP_MODULO = 20,
    SCRIPT_VM_OP_SHIFT_LEFT = 21,
    SCRIPT_VM_OP_SHIFT_RIGHT = 22,
    SCRIPT_VM_OP_AND = 23,
    SCRIPT_VM_OP_OR = 24,
    SCRIPT_VM_OP_XOR = 25,
    SCRIPT_VM_OP_NOT_LOGICAL = 26,
    SCRIPT_VM_OP_NOT_BITWISE = 27,
    SCRIPT_VM_OP_SQRT_INT = 28,
    SCRIPT_VM_OP_INV_SQRT_INT = 29,
    SCRIPT_VM_OP_INVERSE_INT = 30,
    SCRIPT_VM_OP_SIN_INT = 31,
    SCRIPT_VM_OP_COS_INT = 32,
    SCRIPT_VM_OP_ATAN_INT = 33,
    SCRIPT_VM_OP_ATAN2_INT = 34,
    SCRIPT_VM_OP_RANDOM = 35,
    SCRIPT_VM_OP_SET_FIXED = 36,
    SCRIPT_VM_OP_FIXED_TO_INT = 37,
    SCRIPT_VM_OP_FIXED_FLOOR = 38,
    SCRIPT_VM_OP_ADD_FIXED = 39,
    SCRIPT_VM_OP_SUBTRACT_FIXED = 40,
    SCRIPT_VM_OP_MULTIPLY_FIXED = 41,
    SCRIPT_VM_OP_DIVIDE_FIXED = 42,
    SCRIPT_VM_OP_MODULO_FIXED = 43,
    SCRIPT_VM_OP_SQRT_FIXED = 44,
    SCRIPT_VM_OP_INV_SQRT_FIXED = 45,
    SCRIPT_VM_OP_INVERSE_FIXED = 46,
    SCRIPT_VM_OP_SIN_FIXED = 47,
    SCRIPT_VM_OP_COS_FIXED = 48,
    SCRIPT_VM_OP_ATAN_FIXED = 49,
    SCRIPT_VM_OP_ATAN2_FIXED = 50
};

static inline s32 VM_ReadFixedArgument(ScriptVmCommand *command, int index) {
    s32 value = command->arguments[index];

    if ((command->argument_modes & (1 << index)) == 0) {
        value = (value & 0xFFFF) + (command->arguments[index + 1] << 16);
    }
    return value;
}

int VM_ExecuteCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
) {
    int opcode = command->opcode;

    switch (opcode) {
    case SCRIPT_VM_OP_END:
        state->script = 0;
        return SCRIPT_VM_FINISHED;
    case SCRIPT_VM_OP_RETURN:
        if (state->stack_depth != 0) {
            state->stack_depth--;
            state->script = (const u16 *)state->call_stack[state->stack_depth];
            return SCRIPT_VM_CONTINUE;
        }
        state->script = 0;
        return SCRIPT_VM_RETURNED;
    case SCRIPT_VM_OP_JUMP:
        if (command->arguments[0] == 1) {
            state->call_stack[state->stack_depth] = (s32)state->script;
            state->stack_depth++;
        }
        state->script += command->arguments[1];
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_WAIT:
        state->delay = command->arguments[0];
        return SCRIPT_VM_YIELDED;
    case SCRIPT_VM_OP_JUMP_IF:
        if (command->arguments[3] == VM_CheckJumpCondition(
                command->arguments[0],
                command->arguments[1],
                command->arguments[2]
            )) {
            state->script += command->arguments[4];
        }
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_UNKNOWN_05:
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_UNKNOWN_06:
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_UNKNOWN_07:
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_UNKNOWN_08:
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_UNKNOWN_09: {
        s16 byte_index;
        int shift;
        s32 source;

        data_02059d8c(&data_0205003c);
        byte_index = 3 - 2 * (command->argument_modes == 0);
        if (byte_index >= 0) {
            shift = byte_index * 8;
            source = command->arguments[0];
            do {
                u32 bits = (u8)(source >> shift);
                while (bits != 0) {
                    bits >>= 1;
                }
                shift -= 8;
                byte_index--;
            } while (byte_index >= 0);
        }
        data_02059d8c(&data_02050040);
        return SCRIPT_VM_CONTINUE;
    }
    case SCRIPT_VM_OP_PUSH:
        state->call_stack[state->stack_depth] = command->arguments[0];
        state->stack_depth++;
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_POP:
        state->stack_depth--;
        VM_WriteVariable(
            command->result_variable,
            state->call_stack[state->stack_depth],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_LOOP: {
        s32 *value = &state->loop_stack[state->stack_depth];
        u16 mode = command->arguments[0] & 7;

        if (mode == 3) {
            (*value)++;
        } else if (mode == 4) {
            (*value)--;
        }
        if (VM_CheckJumpCondition(
                command->arguments[1], command->arguments[2], *value
            )) {
            state->script += command->arguments[3];
            if ((command->arguments[0] & 0x18) == 0x10) {
                state->stack_depth--;
                return SCRIPT_VM_CONTINUE;
            }
        } else if ((command->arguments[0] & 0x18) == 0x18) {
            state->stack_depth--;
            return SCRIPT_VM_CONTINUE;
        }
        if (mode == 1) {
            (*value)++;
        } else if (mode == 2) {
            (*value)--;
        }
        return SCRIPT_VM_CONTINUE;
    }
    case SCRIPT_VM_OP_READ_TABLE:
        VM_WriteVariable(
            command->result_variable,
            *((const s32 *)(state->script + command->arguments[0] + 2)
              + command->arguments[1]),
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_READ_SCRIPT:
        VM_WriteVariable(
            command->result_variable,
            *(const s32 *)(state->script + command->arguments[0]),
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SET:
        VM_WriteVariable(command->result_variable, command->arguments[0], vm, state);
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_ADD:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] + command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SUBTRACT:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] - command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_MULTIPLY:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] * command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_DIVIDE:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] / command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_MODULO:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] % command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SHIFT_LEFT:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] << command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SHIFT_RIGHT:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] >> command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_AND:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] & command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_OR:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] | command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_XOR:
        VM_WriteVariable(
            command->result_variable,
            command->arguments[0] ^ command->arguments[1],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_NOT_LOGICAL:
        VM_WriteVariable(
            command->result_variable, command->arguments[0] == 0, vm, state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_NOT_BITWISE:
        VM_WriteVariable(
            command->result_variable, ~command->arguments[0], vm, state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SQRT_INT:
        VM_WriteVariable(
            command->result_variable,
            FX_Sqrt(command->arguments[0] << 12) / 4096,
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_INV_SQRT_INT:
        VM_WriteVariable(
            command->result_variable,
            FX_InvSqrt(command->arguments[0] << 12) / 4096,
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_INVERSE_INT:
        VM_WriteVariable(
            command->result_variable,
            FX_Inv(command->arguments[0] << 12) / 4096,
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SIN_INT:
        VM_WriteVariable(
            command->result_variable,
            FX_SinCosTable_[
                2 * ((((command->arguments[0] << 16) / 360) & 0xFFFF) >> 4)
            ],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_COS_INT:
        VM_WriteVariable(
            command->result_variable,
            FX_SinCosTable_[
                2 * ((((command->arguments[0] << 16) / 360) & 0xFFFF) >> 4) + 1
            ],
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_ATAN_INT:
        VM_WriteVariable(
            command->result_variable,
            360 * FX_AtanIdx(command->arguments[0]) / 0x10000,
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_ATAN2_INT:
        VM_WriteVariable(
            command->result_variable,
            360 * FX_Atan2Idx(command->arguments[0], command->arguments[1])
                / 0x10000,
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_RANDOM:
        VM_WriteVariable(
            command->result_variable,
            Random_NextModulo(command->arguments[0]),
            vm,
            state
        );
        return SCRIPT_VM_CONTINUE;
    case SCRIPT_VM_OP_SET_FIXED:
    case SCRIPT_VM_OP_FIXED_TO_INT:
    case SCRIPT_VM_OP_FIXED_FLOOR:
    case SCRIPT_VM_OP_SQRT_FIXED:
    case SCRIPT_VM_OP_INV_SQRT_FIXED:
    case SCRIPT_VM_OP_INVERSE_FIXED:
    case SCRIPT_VM_OP_SIN_FIXED:
    case SCRIPT_VM_OP_COS_FIXED:
    case SCRIPT_VM_OP_ATAN_FIXED: {
        s32 value = VM_ReadFixedArgument(command, 0);

        switch (opcode) {
        case SCRIPT_VM_OP_SET_FIXED:
            VM_WriteVariable(command->result_variable, value, vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_FIXED_TO_INT:
            VM_WriteVariable(command->result_variable, value >> 12, vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_FIXED_FLOOR:
            VM_WriteVariable(command->result_variable, value & ~0xFFF, vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_SQRT_FIXED:
            VM_WriteVariable(command->result_variable, FX_Sqrt(value), vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_INV_SQRT_FIXED:
            VM_WriteVariable(command->result_variable, FX_InvSqrt(value), vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_INVERSE_FIXED:
            VM_WriteVariable(command->result_variable, FX_Inv(value), vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_SIN_FIXED:
            VM_WriteVariable(
                command->result_variable,
                FX_SinCosTable_[2 * (((value * 16 / 360) & 0xFFFF) >> 4)],
                vm,
                state
            );
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_COS_FIXED:
            VM_WriteVariable(
                command->result_variable,
                FX_SinCosTable_[2 * (((value * 16 / 360) & 0xFFFF) >> 4) + 1],
                vm,
                state
            );
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_ATAN_FIXED:
            VM_WriteVariable(
                command->result_variable,
                360 * FX_AtanIdx(value) / 16,
                vm,
                state
            );
            return SCRIPT_VM_CONTINUE;
        default:
            break;
        }
        break;
    }
    case SCRIPT_VM_OP_ADD_FIXED:
    case SCRIPT_VM_OP_SUBTRACT_FIXED:
    case SCRIPT_VM_OP_MULTIPLY_FIXED:
    case SCRIPT_VM_OP_DIVIDE_FIXED:
    case SCRIPT_VM_OP_MODULO_FIXED:
    case SCRIPT_VM_OP_ATAN2_FIXED: {
        s32 left = VM_ReadFixedArgument(command, 0);
        s32 right = VM_ReadFixedArgument(command, 2);

        switch (opcode) {
        case SCRIPT_VM_OP_ADD_FIXED:
            VM_WriteVariable(command->result_variable, left + right, vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_SUBTRACT_FIXED:
            VM_WriteVariable(command->result_variable, left - right, vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_MULTIPLY_FIXED:
            VM_WriteVariable(
                command->result_variable,
                (s32)(((s64)left * right + 0x800) >> 12),
                vm,
                state
            );
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_DIVIDE_FIXED:
            VM_WriteVariable(command->result_variable, FX_Div(left, right), vm, state);
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_MODULO_FIXED:
            VM_WriteVariable(
                command->result_variable, FX_ModS32(left, right), vm, state
            );
            return SCRIPT_VM_CONTINUE;
        case SCRIPT_VM_OP_ATAN2_FIXED:
            VM_WriteVariable(
                command->result_variable,
                360 * FX_Atan2Idx(left, right) / 16,
                vm,
                state
            );
            return SCRIPT_VM_CONTINUE;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    return vm->command_handler(vm, state, command);
}
