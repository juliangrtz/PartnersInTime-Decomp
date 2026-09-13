#include "pause_scene_internal.h"

extern "C" {
void func_ov005_02066358(PauseMenuElement *, void (*)(PauseMenuElement *), int);
void func_ov007_0206e27c(PauseMenuElement *);
}

extern "C" s32 PauseTransition_GetProgress(void)
{
    return *(s32 *)data_ov007_0208e1e8;
}

extern "C" void PauseScene_RequestExit(PauseSceneTask *scene, int mode)
{
    scene->phase = 5;
    if (mode == 0)
        func_ov005_02066358(scene->menu,
            (void (*)(PauseMenuElement *))PauseScene_FadeOutTask, 0);
    else
        func_ov005_02066358(scene->menu, func_ov007_0206e27c, 0);
}
