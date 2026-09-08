#ifndef PIT_GAME_SCENE_OBJECT_H
#define PIT_GAME_SCENE_OBJECT_H
#include <game/scene_script.h>
typedef struct BattleModel BattleModel;
#ifdef __cplusplus
extern "C" {
#endif
void SceneObject_SetProperty(u32 id, int property, int value);
void SceneObjects_SwapSlots(u32 first, u32 second);
void SceneObject_SetAnimation(SceneObject *object, int animation, int secondary);
void SceneObject_SetAnimationById(u32 id, int animation, int secondary);
void SceneObject_SetSecondaryAnimation(SceneObject *object, int animation, int mode);
int SceneObject_IsMotionActive(SceneObject *object, int channel);
int SceneObject_IsMotionActiveById(u32 id, int channel);
void SceneObject_SetResourceId(u32 id, u32 resource);
int SceneResource_IsReady(int slot);
int SceneResource_LoadSlot(u32 slot, u32 resource);
SceneScriptState *SceneScript_GetObjectState(u32 id);
int SceneObject_UpdateAnimationLimit(SceneObject *object);
void SceneObject_SetAnimationLimit(SceneObject *object, int value, int flag);
void SceneObject_SetFlipY(u32 id, int enabled);
void SceneObject_SetFacingLeft(u32 id, int enabled);
BattleModel *SceneObject_GetActiveModel(SceneObject *object);
BattleModel *SceneObject_GetModelById(u32 id);
void SceneModel_Stop(BattleModel *model);
void SceneModel_Initialize(BattleModel *model, int engine, int slot);
#ifdef __cplusplus
}
#endif
#endif
