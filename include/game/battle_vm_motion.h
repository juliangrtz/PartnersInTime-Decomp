#ifndef PIT_GAME_BATTLE_VM_MOTION_H
#define PIT_GAME_BATTLE_VM_MOTION_H

/*
 * The motion commands the battle scripts start an object moving with.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <game/script_vm.h>

void BattleVm_StartVerticalMotion(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result);
void BattleVm_MoveWithVerticalDuration(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result);

void BattleVm_StartBallisticMotion(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result);
void BattleVm_StartScaledAcceleratedMotion(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result);
void BattleVm_StartMotionWithPeakDistance(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result);

#ifdef __cplusplus
}
#endif

#endif
