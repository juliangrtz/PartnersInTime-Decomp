#include <game/scene_motion.h>

/* Argument-mode bits: a clear bit means the pair of script words still has to
   be joined into one fixed-point value. */
enum {
    VM_MOTION_MODE_ARGUMENT_4 = 0x10,
    VM_MOTION_MODE_ARGUMENT_6 = 0x40,
    VM_MOTION_MODE_ARGUMENT_7 = 0x80,
    VM_MOTION_MODE_ARGUMENT_8 = 0x100,
    VM_MOTION_MODE_ARGUMENT_9 = 0x200,
    VM_MOTION_CURVE = 4
};

extern int func_ov007_020866a0(void *object, u16 owner, s32 x, s32 y, s32 z,
                               int curve);
extern int func_ov007_02086828(void *object, u16 owner, s32 x, s32 y, s32 z);

void func_ov007_020881d8(ScriptVm *vm, ScriptVmState *state,
                         ScriptVmCommand *command, int store_result)
{
    void *object = SceneObject_GetById((u16)command->arguments[0]);
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_4)) {
        command->arguments[4] = (command->arguments[4] & 0xffff) |
                                (command->arguments[5] << 16);
    }
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_6)) {
        command->arguments[6] = (command->arguments[6] & 0xffff) |
                                (command->arguments[7] << 16);
    }
    if ((u16)command->arguments[2] == VM_MOTION_CURVE) {
        if (store_result) {
            VM_WriteVariable(command->result_variable,
                func_ov007_020866a0(object, (u16)command->arguments[1],
                    command->arguments[4] / 16, command->arguments[6] / 16,
                    command->arguments[3], 1),
                vm, state);
        } else {
            func_ov007_020866a0(object, (u16)command->arguments[1],
                command->arguments[4] / 16, command->arguments[6] / 16,
                command->arguments[3], 1);
        }
    } else {
        if (store_result) {
            VM_WriteVariable(command->result_variable,
                func_ov007_02086828(object, (u16)command->arguments[1],
                    command->arguments[4] / 16, command->arguments[6],
                    command->arguments[3]),
                vm, state);
        } else {
            func_ov007_02086828(object, (u16)command->arguments[1],
                command->arguments[4] / 16, command->arguments[6],
                command->arguments[3]);
        }
    }
}

void func_ov007_02087fdc(ScriptVm *vm, ScriptVmState *state,
                         ScriptVmCommand *command, int store_result)
{
    void *object;
    int result;
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_7)) {
        command->arguments[7] = (command->arguments[7] & 0xffff) |
                                (command->arguments[8] << 16);
    }
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_9)) {
        command->arguments[9] = (command->arguments[9] & 0xffff) |
                                (command->arguments[10] << 16);
    }
    object = SceneObject_GetById((u16)command->arguments[0]);
    if (store_result) {
        if (command->arguments[3] == VM_MOTION_CURVE) {
            result = func_ov007_020866a0(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9] / 16,
                command->arguments[6], 1);
        } else {
            result = func_ov007_02086828(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9],
                command->arguments[6]);
        }
        SceneObject_MoveTo(object, (u16)command->arguments[1],
            command->arguments[4], command->arguments[5],
            command->arguments[6], result);
        VM_WriteVariable(command->result_variable, result, vm, state);
    } else {
        if (command->arguments[3] == VM_MOTION_CURVE) {
            result = func_ov007_020866a0(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9] / 16,
                command->arguments[6], 1);
        } else {
            result = func_ov007_02086828(object, (u16)command->arguments[2],
                command->arguments[7] / 16, command->arguments[9],
                command->arguments[6]);
        }
        SceneObject_MoveTo(object, (u16)command->arguments[1],
            command->arguments[4], command->arguments[5],
            command->arguments[6], result);
    }
}

void func_ov007_02087ebc(ScriptVm *vm, ScriptVmState *state,
                         ScriptVmCommand *command, int store_result)
{
    void *object = SceneObject_GetById((u16)command->arguments[0]);
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_7)) {
        command->arguments[7] = ((command->arguments[7] & 0xffff) |
                                 (command->arguments[8] << 16)) / 16;
    }
    if (store_result) {
        VM_WriteVariable(command->result_variable,
            SceneObject_StartMotionWithPeakDistance(object, (u16)command->arguments[1],
                command->arguments[2], command->arguments[3],
                command->arguments[4], command->arguments[5],
                command->arguments[6], command->arguments[7],
                (u16)command->arguments[9]),
            vm, state);
    } else {
        SceneObject_StartMotionWithPeakDistance(object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3],
            command->arguments[4], command->arguments[5],
            command->arguments[6], command->arguments[7],
            (u16)command->arguments[9]);
    }
}

void func_ov007_02087d6c(ScriptVm *vm, ScriptVmState *state,
                         ScriptVmCommand *command, int store_result)
{
    void *object = SceneObject_GetById((u16)command->arguments[0]);
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_6)) {
        command->arguments[6] = ((command->arguments[6] & 0xffff) |
                                 (command->arguments[7] << 16)) / 16;
    }
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_8)) {
        command->arguments[8] = ((command->arguments[8] & 0xffff) |
                                 (command->arguments[9] << 16)) / 16;
    }
    if (store_result) {
        VM_WriteVariable(command->result_variable,
            SceneObject_StartScaledAcceleratedMotion(object, (u16)command->arguments[1],
                command->arguments[2], command->arguments[3],
                command->arguments[4], command->arguments[5],
                command->arguments[6], command->arguments[8],
                (u16)command->arguments[10]),
            vm, state);
    } else {
        SceneObject_StartScaledAcceleratedMotion(object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3],
            command->arguments[4], command->arguments[5],
            command->arguments[6], command->arguments[8],
            (u16)command->arguments[10]);
    }
}

void func_ov007_02087c4c(ScriptVm *vm, ScriptVmState *state,
                         ScriptVmCommand *command, int store_result)
{
    void *object = SceneObject_GetById((u16)command->arguments[0]);
    if (!(command->argument_modes & VM_MOTION_MODE_ARGUMENT_6)) {
        command->arguments[6] = ((command->arguments[6] & 0xffff) |
                                 (command->arguments[7] << 16)) / 16;
    }
    if (store_result) {
        VM_WriteVariable(command->result_variable,
            SceneMotion_StartBallistic(object, (u16)command->arguments[1],
                command->arguments[2], command->arguments[3],
                command->arguments[4], command->arguments[5],
                command->arguments[6], command->arguments[8],
                (u16)command->arguments[9]),
            vm, state);
    } else {
        SceneMotion_StartBallistic(object, (u16)command->arguments[1],
            command->arguments[2], command->arguments[3],
            command->arguments[4], command->arguments[5],
            command->arguments[6], command->arguments[8],
            (u16)command->arguments[9]);
    }
}
