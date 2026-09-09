#ifndef PIT_GAME_SCENE_VM_STATE_H
#define PIT_GAME_SCENE_VM_STATE_H
#include <game/scene_script.h>
enum SceneSaveProperty {
    SCENE_SAVE_BASE_MAX_HP = 0,
    SCENE_SAVE_BASE_POWER = 1,
    SCENE_SAVE_BASE_DEFENSE = 2,
    SCENE_SAVE_BASE_SPEED = 3,
    SCENE_SAVE_BASE_STACHE = 4,
    SCENE_SAVE_MAX_HP = 5,
    SCENE_SAVE_CURRENT_HP = 6,
    SCENE_SAVE_POWER = 7,
    SCENE_SAVE_DEFENSE = 8,
    SCENE_SAVE_SPEED = 9,
    SCENE_SAVE_STACHE = 10,
    SCENE_SAVE_LEVEL = 11,
    SCENE_SAVE_EXPERIENCE = 12,
    SCENE_SAVE_EXPERIENCE_TO_NEXT_LEVEL = 13,
    SCENE_SAVE_CLOTHING = 14,
    SCENE_SAVE_BADGE = 15,
    SCENE_SAVE_COINS = 16,
    SCENE_SAVE_BEANS = 17
};
#ifdef __cplusplus
extern "C" {
#endif
void SceneObject_UpdatePath(SceneObject *object, SceneMotionChannel *channel);
void SceneInput_ClearPersistentMask(void);
void SceneInput_SetDisabledMask(int mask);
void SceneInput_SetAuxiliaryMask(int mask);
void SceneInput_SetRejectedMask(int mask);
void SceneInput_ClearRejectedMask(void);
int SceneMenu_GetSelectionIndex(void);
int SceneMenu_GetSelectedItem(void);
int SceneMenu_GetSelectedMember(void);
int SceneVM_GetMenuValue(int property);
int SceneVM_GetSaveValue(int property, int member);
#ifdef __cplusplus
}
#endif
#endif
