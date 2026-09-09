#ifndef PIT_SCENE_VM_STATE_INTERNAL_H
#define PIT_SCENE_VM_STATE_INTERNAL_H
#include <game/scene_vm_state.h>
#include <game/save_data.h>
#include <game/overlay007_party.h>
#include <game/pause_scene.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct SceneSaveValues {
    u8 unknown_000[1016];
    SavePartyMember members[4];
    u32 coins;
    u16 beans;
    u16 unknown_48e;
} SceneSaveValues;
typedef char SceneSaveValues_SizeCheck[sizeof(SceneSaveValues) == 1168 ? 1 : -1];
typedef struct SceneMenuContext {
    u8 unknown_000[0x1a];
    u16 persistent_input, transient_input, disabled_input, rejected_input, auxiliary_input;
    u8 unknown_024[0xee];
    s8 menu;
    u8 unknown_113[2];
    s8 member;
    u8 unknown_116[2];
} SceneMenuContext;
typedef char SceneMenuContext_SizeCheck[sizeof(SceneMenuContext) == 0x118 ? 1 : -1];
extern u8 gSceneMenuSubmenus[];
#define SCENE_MENU (*(SceneMenuContext *)data_ov007_020905f0)
#define SCENE_SAVE_VALUES (*(SceneSaveValues *)gSaveData)
extern Overlay7Party *data_ov007_0208e1e4;
extern PauseSceneTask *data_ov007_0208e1e0;
extern u8 func_ov007_02075408(Overlay7Party *);
extern u8 func_ov007_02075400(Overlay7Party *);
extern u8 func_ov007_02075324(Overlay7Party *, int);
#ifdef __cplusplus
}
#endif
#endif
