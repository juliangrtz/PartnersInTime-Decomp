extern "C" {
#include <game/battle_scene.h>
#include <game/battle_vm_motion.h>
}
#include "battle_vm_operands.h"

/* Definitions follow MWCC's reverse emission order. Keep result and non-result
 * calls in their native branches so argument loads retain their ABI schedule. */
enum { BATTLE_VM_VERTICAL_SOLVER_ACCELERATION = 4 };

extern "C" {
void BattleVm_StartVerticalMotion(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result)
{
    BattleSceneObject *object = BattleSceneObject_GetById((u16)command->arguments[0]);
    BattleVm_DecodePackedArgument(command, 4);
    BattleVm_DecodePackedArgument(command, 6);
    if ((u16)command->arguments[2] == BATTLE_VM_VERTICAL_SOLVER_ACCELERATION) {
        if (write_result) {
            VM_WriteVariable(command->result_variable,
                BattleSceneObject_StartVerticalMotionTo(object, (u16)command->arguments[1],
                    command->arguments[4] / 16, command->arguments[6] / 16,
                    command->arguments[3], 1), vm, state);
        } else {
            BattleSceneObject_StartVerticalMotionTo(object, (u16)command->arguments[1],
                command->arguments[4] / 16, command->arguments[6] / 16,
                command->arguments[3], 1);
        }
    } else {
        if (write_result) {
            VM_WriteVariable(command->result_variable,
                BattleSceneObject_StartVerticalArc(object, (u16)command->arguments[1],
                    command->arguments[4] / 16, command->arguments[6],
                    command->arguments[3]), vm, state);
        } else {
            BattleSceneObject_StartVerticalArc(object, (u16)command->arguments[1],
                command->arguments[4] / 16, command->arguments[6], command->arguments[3]);
        }
    }
}

void BattleVm_MoveWithVerticalDuration(ScriptVm *vm, ScriptVmState *state,
    ScriptVmCommand *command, int write_result)
{
    BattleVm_DecodePackedArgument(command, 7);
    BattleVm_DecodePackedArgument(command, 9);
    BattleSceneObject *object = BattleSceneObject_GetById((u16)command->arguments[0]);
    if (write_result) {
        int duration;
        if (command->arguments[3] == BATTLE_VM_VERTICAL_SOLVER_ACCELERATION) {
            duration = BattleSceneObject_StartVerticalMotionTo(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9] / 16,
                command->arguments[6], 1);
        } else {
            duration = BattleSceneObject_StartVerticalArc(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9], command->arguments[6]);
        }
        BattleSceneObject_MoveTo(object, (u16)command->arguments[1], command->arguments[4],
            command->arguments[5], command->arguments[6], duration);
        VM_WriteVariable(command->result_variable, duration, vm, state);
    } else {
        int duration;
        if (command->arguments[3] == BATTLE_VM_VERTICAL_SOLVER_ACCELERATION) {
            duration = BattleSceneObject_StartVerticalMotionTo(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9] / 16,
                command->arguments[6], 1);
        } else {
            duration = BattleSceneObject_StartVerticalArc(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9], command->arguments[6]);
        }
        BattleSceneObject_MoveTo(object, (u16)command->arguments[1], command->arguments[4],
            command->arguments[5], command->arguments[6], duration);
    }
}

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
