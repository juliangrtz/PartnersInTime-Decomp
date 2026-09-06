#ifndef PIT_GAME_SCENE_SCRIPT_H
#define PIT_GAME_SCENE_SCRIPT_H

#include <game/script_vm.h>

typedef struct SceneScriptState SceneScriptState;
typedef struct SceneTask SceneTask;

enum SceneScriptOwner {
    SCENE_SCRIPT_OWNER_PRIMARY = 1,
    SCENE_SCRIPT_OWNER_SECONDARY = 4,
    SCENE_SCRIPT_OWNER_OBJECT = 0x4000,
    SCENE_SCRIPT_OWNER_ID_MASK = 0x0FFF,
    SCENE_SCRIPT_OWNER_TYPE_MASK = 0xF000
};

struct SceneScriptState {
    ScriptVmState vm_state;
    u32 unknown_ac;
    u16 owner;
    union {
        u16 raw;
        struct {
            u16 paused : 1;
            u16 unknown_1_15 : 15;
        } bits;
    } flags;
    u16 parent_owner;
    u16 parent_object_id;
    const u16 *queued_script;
    u16 queued_parent_owner;
    u16 queued_parent_object_id;
};

struct SceneTask {
    u8 unknown_00[0x28];
    void *argument;
    u16 owner_id;
};

typedef char SceneScriptState_SizeCheck[
    sizeof(SceneScriptState) == 0xC0 ? 1 : -1
];

SceneScriptState *SceneScript_GetState(u32 owner);
void *SceneObject_GetById(u32 object_id);
void SceneScript_Stop(u32 owner);
void SceneScript_StartObjectScript(
    u32 object_id,
    const u16 *script,
    const SceneScriptState *parent,
    int queue_if_running
);

#endif
