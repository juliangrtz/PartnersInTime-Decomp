#ifndef PIT_GAME_SCENE_SCRIPT_H
#define PIT_GAME_SCENE_SCRIPT_H

#include <game/script_vm.h>

typedef struct SceneScriptState SceneScriptState;
typedef struct SceneTask SceneTask;
typedef struct SceneObject SceneObject;

typedef union SceneObjectFlags {
    u16 raw;
    struct {
        u16 unknown_0_2 : 3;
        u16 ready : 1;
        u16 unknown_4_15 : 12;
    } bits;
} SceneObjectFlags;

typedef struct SceneSpriteTaskData {
    u8 unknown_00[0x14];
    u32 attributes_0;
    u32 attributes_1;
    s32 x;
    s32 y;
    s32 target_x;
    s32 target_y;
} SceneSpriteTaskData;

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

struct SceneObject {
    u8 unknown_000[0x04];
    s16 x;
    s16 y;
    s16 base_y;
    u8 unknown_00a[0xB2];
    SceneObject *render_next;
    u16 resource_id;
    u16 unknown_0c2;
    void *primary_renderable;
    void *secondary_renderable;
    void *resource;
    u8 unknown_0d0[0x04];
    u16 animation_id;
    u8 unknown_0d6[0x02];
    s16 render_height;
    s16 default_motion_speed;
    u16 object_id;
    u16 linked_object_id;
    SceneObjectFlags flags;
    s8 render_offset_x;
    s8 render_offset_y;
};

typedef char SceneScriptState_SizeCheck[
    sizeof(SceneScriptState) == 0xC0 ? 1 : -1
];
typedef char SceneObject_SizeCheck[sizeof(SceneObject) == 0xE4 ? 1 : -1];

SceneScriptState *SceneScript_GetState(u32 owner);
void *SceneObject_GetById(u32 object_id);
void SceneScript_Stop(u32 owner);
void SceneScript_StartObjectScript(
    u32 object_id,
    const u16 *script,
    const SceneScriptState *parent,
    int queue_if_running
);
void SceneObjects_UpdateAndDraw(u8 *manager);
void SceneScripts_UpdateGlobal(u8 *manager);
void SceneObjects_ClearPrimaryRange(u8 *manager);
void SceneObjects_ClearSecondaryRange(u8 *manager);
void SceneManager_ClearRuntime(void *manager);
int SceneSpriteTask_Update(void *task);
int SceneVm_DispatchCommand(
    ScriptVm *vm, ScriptVmState *state, ScriptVmCommand *command
);

#endif
