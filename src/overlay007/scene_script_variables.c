#include <game/script_vm.h>

typedef struct SceneScriptManager {
    u8 unknown_0000[0x37EC];
    s32 shared_variables[32];
} SceneScriptManager;

extern u16 data_0206032c[];
extern u16 data_ov007_020905f0[];
extern SceneScriptManager *data_ov007_020a6b90;

enum SceneScriptVariable {
    SCENE_SCRIPT_VAR_OWNER_ID = 0x7000,
    SCENE_SCRIPT_VAR_OWNER_TYPE = 0x7001,
    SCENE_SCRIPT_VAR_INPUT_0 = 0x7004,
    SCENE_SCRIPT_VAR_INPUT_1 = 0x7005,
    SCENE_SCRIPT_VAR_INPUT_3 = 0x7006,
    SCENE_SCRIPT_VAR_INPUT_0_WITH_LATCH = 0x7007,
    SCENE_SCRIPT_VAR_INPUT_1_WITH_LATCH = 0x7008,
    SCENE_SCRIPT_VAR_INPUT_3_WITH_LATCH = 0x7009,
    SCENE_SCRIPT_VAR_SHARED_FIRST = 0x7010,
    SCENE_SCRIPT_VAR_SHARED_LAST = 0x702F
};

/* Metrowerks emits C functions in reverse source order. */
s32 SceneScriptVM_ReadVariable(u16 variable, ScriptVm *vm,
                               ScriptVmState *state, u32 index) {
    switch (variable) {
    case SCENE_SCRIPT_VAR_OWNER_ID:
        return *(u16 *)((u8 *)state + 0xB0) & 0xFFF;
    case SCENE_SCRIPT_VAR_OWNER_TYPE:
        return (s32)*(u16 *)((u8 *)state + 0xB0) >> 12;
    case SCENE_SCRIPT_VAR_INPUT_0:
        return data_ov007_020905f0[0];
    case SCENE_SCRIPT_VAR_INPUT_1:
        return data_ov007_020905f0[2];
    case SCENE_SCRIPT_VAR_INPUT_3:
        return data_ov007_020905f0[1];
    case SCENE_SCRIPT_VAR_INPUT_0_WITH_LATCH:
        return data_0206032c[0] | data_ov007_020905f0[3];
    case SCENE_SCRIPT_VAR_INPUT_1_WITH_LATCH:
        return data_0206032c[1] | data_ov007_020905f0[4];
    case SCENE_SCRIPT_VAR_INPUT_3_WITH_LATCH:
        return data_0206032c[3] | data_ov007_020905f0[6];
    default:
        if (variable >= SCENE_SCRIPT_VAR_SHARED_FIRST
            && variable <= SCENE_SCRIPT_VAR_SHARED_LAST) {
            return data_ov007_020a6b90
                ->shared_variables[variable - SCENE_SCRIPT_VAR_SHARED_FIRST];
        }
        return 0;
    }
}

void SceneScriptVM_WriteVariable(u16 variable, s32 value, ScriptVm *vm,
                                 ScriptVmState *state) {
    if (variable >= SCENE_SCRIPT_VAR_SHARED_FIRST
        && variable <= SCENE_SCRIPT_VAR_SHARED_LAST) {
        data_ov007_020a6b90
            ->shared_variables[variable - SCENE_SCRIPT_VAR_SHARED_FIRST] = value;
    }
}
