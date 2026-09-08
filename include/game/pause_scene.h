#ifndef PIT_GAME_PAUSE_SCENE_H
#define PIT_GAME_PAUSE_SCENE_H
#include <game/task.h>
typedef struct PauseMenuElement PauseMenuElement;
typedef struct PauseSceneTask {
    GameTask task;
    u8 unknown18[20];
    GameIrqTask *irq;
    s32 phase;
    PauseMenuElement *menu;
} PauseSceneTask;
typedef char PauseSceneTaskSizeCheck[sizeof(PauseSceneTask) == 56 ? 1 : -1];
#ifdef __cplusplus
extern "C" {
#endif
GameIrqTask *PauseSceneIrq_Destroy(GameIrqTask *task);
GameIrqTask *PauseSceneIrq_Delete(GameIrqTask *task);
PauseSceneTask *PauseScene_Destroy(PauseSceneTask *task);
PauseSceneTask *PauseScene_Init(PauseSceneTask *task, u32 priority, u32 unused);
GameIrqTask *PauseSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent);
#ifdef __cplusplus
}
#endif
#endif
