#include "frontend_scene_internal.h"

TitleSceneTask *TitleScene_Destroy(TitleSceneTask *task)
{
    int i;
    task->base.vtable = &data_ov006_0207b51c;
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    func_02009058(0);
    func_02009058(1);
    func_ov005_02065f10();
    func_ov005_02069928();
    func_ov005_02069660();
    func_ov005_02068c94();
    func_ov005_020699f0();
    func_ov005_020690dc();
    func_ov005_02066624();
    Overlay5Display_ResetEngine(0);
    Overlay5Display_ResetEngine(1);
    func_ov005_0206781c(2, 16);
    REG32(0x04000000) &= ~0x1F00;
    REG32(0x04001000) &= ~0x1F00;
    GameDisplay_SetBackgroundColor(0, 0);
    data_ov006_0207c4e8 = 0;
    GameTask_DestroyBase(&task->base);
    return task;
}

TitleSceneTask *TitleScene_Delete(TitleSceneTask *task)
{
    int i;
    task->base.vtable = &data_ov006_0207b51c;
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    func_02009058(0);
    func_02009058(1);
    func_ov005_02065f10();
    func_ov005_02069928();
    func_ov005_02069660();
    func_ov005_02068c94();
    func_ov005_020699f0();
    func_ov005_020690dc();
    func_ov005_02066624();
    Overlay5Display_ResetEngine(0);
    Overlay5Display_ResetEngine(1);
    func_ov005_0206781c(2, 16);
    REG32(0x04000000) &= ~0x1F00;
    REG32(0x04001000) &= ~0x1F00;
    GameDisplay_SetBackgroundColor(0, 0);
    data_ov006_0207c4e8 = 0;
    GameTask_DestroyBase(&task->base);
    GameHeap_Delete(task);
    return task;
}
