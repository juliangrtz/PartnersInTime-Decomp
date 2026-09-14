extern "C" {
#include <game/battle_scene.h>
#include <game/battle_vm_motion.h>
}
#include "battle_vm_operands.h"

/* Definitions follow MWCC's reverse emission order. Keep result and non-result
 * calls in their native branches so argument loads retain their ABI schedule. */
extern "C" {
void BattleVm_StartMotionWithPeakDistance(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result)
{
    BattleSceneObject *object = BattleSceneObject_GetById((u16)command->arguments[0]);
    BattleVm_DecodeFixedArgument(command, 7);
    if (write_result) {
        VM_WriteVariable(command->result_variable, BattleSceneObject_StartMotionWithPeakDistance(
            object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3], command->arguments[4],
            command->arguments[5], command->arguments[6], command->arguments[7], (u16)command->arguments[9]), vm, state);
    } else {
        BattleSceneObject_StartMotionWithPeakDistance(
            object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3], command->arguments[4],
            command->arguments[5], command->arguments[6], command->arguments[7], (u16)command->arguments[9]);
    }
}

void BattleVm_StartScaledAcceleratedMotion(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result)
{
    BattleSceneObject *object = BattleSceneObject_GetById((u16)command->arguments[0]);
    BattleVm_DecodeFixedArgument(command, 6);
    BattleVm_DecodeFixedArgument(command, 8);
    if (write_result) {
        VM_WriteVariable(command->result_variable, BattleSceneObject_StartScaledAcceleratedMotion(
            object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3], command->arguments[4],
            command->arguments[5], command->arguments[6], command->arguments[8], (u16)command->arguments[10]), vm, state);
    } else {
        BattleSceneObject_StartScaledAcceleratedMotion(
            object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3], command->arguments[4],
            command->arguments[5], command->arguments[6], command->arguments[8], (u16)command->arguments[10]);
    }
}

void BattleVm_StartBallisticMotion(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result)
{
    BattleSceneObject *object = BattleSceneObject_GetById((u16)command->arguments[0]);
    BattleVm_DecodeFixedArgument(command, 6);
    if (write_result) {
        VM_WriteVariable(command->result_variable, BattleMotion_StartBallistic(
            object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3], command->arguments[4],
            command->arguments[5], command->arguments[6], command->arguments[8], (u16)command->arguments[9]), vm, state);
    } else {
        BattleMotion_StartBallistic(
            object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3], command->arguments[4],
            command->arguments[5], command->arguments[6], command->arguments[8], (u16)command->arguments[9]);
    }
}

}
