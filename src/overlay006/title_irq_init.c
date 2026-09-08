#include "frontend_scene_internal.h"

GameIrqTask *TitleSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov006_0207b508;
    return task;
}

void TitleScene_UploadFrame(void)
{
    if (func_ov005_02065f00()) {
        func_ov005_0206651c(11, 11);
        func_ov005_0206642c(11, 11);
        func_ov005_02068f84(0);
        func_ov005_02068f84(1);
        GameSpritePalette_UploadScreen(0);
        GameSpritePalette_UploadScreen(1);
        GameDisplay_CopyOam(3);
        GameDisplay_ClearOam();
        func_ov005_02065eec();
    }
}
