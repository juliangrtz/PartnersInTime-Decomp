#ifndef PIT_BATTLE_VM_OPERANDS_H
#define PIT_BATTLE_VM_OPERANDS_H

/*
 * How the battle VM's packed operands are decoded: two halfwords combined as
 * ((low & 0xFFFF) | (high << 16)), with a signed division that truncates toward
 * zero rather than an arithmetic shift.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <game/script_vm.h>

static inline u32 BattleVm_PackHalfwords(s32 low, s32 high) {
    return ((u32)low & 0xFFFF) | ((u32)high << 16);
}

static inline void BattleVm_DecodeFixedArgument(ScriptVmCommand *command,
                                               int argument_index) {
    if ((command->argument_modes & (1 << argument_index)) == 0) {
        command->arguments[argument_index] =
            (s32)BattleVm_PackHalfwords(
                command->arguments[argument_index],
                command->arguments[argument_index + 1]) / 16;
    }
}

static inline void BattleVm_DecodePackedArgument(ScriptVmCommand *command,
                                                int argument_index) {
    if ((command->argument_modes & (1 << argument_index)) == 0) {
        command->arguments[argument_index] = BattleVm_PackHalfwords(
            command->arguments[argument_index], command->arguments[argument_index + 1]);
    }
}

#ifdef __cplusplus
}
#endif

#endif
