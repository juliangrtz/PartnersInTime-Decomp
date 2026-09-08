#include "save_menu_internal.h"

SaveSceneTask *EraseScene_Init(SaveSceneTask *task, u32 priority, u32 unused)
{
    Overlay5Archive *archive;
    GameIrqTask *irq;
    GameTask_Init(&task->base, priority, unused, 0);
    task->base.vtable = &data_ov008_02078264;
    data_ov008_0207aa98 = task;
    func_ov005_02066128(0);
    Overlay5Display_ResetEngine(0);
    Overlay5Display_ResetEngine(1);
    func_ov005_0206781c(2, 16);
    REG16(0x04000304) &= ~0x8000;
    func_02035fd0(1);
    func_0203613c(2);
    func_020359c4(8);
    func_02035a40(4);
    GX_SetGraphicsMode(1, 3, 0);
    GXS_SetGraphicsMode(0);
    Overlay5Display_SetObjTileMappingMode(0, 1048592);
    Overlay5Display_SetObjTileMappingMode(1, 1048592);
    Overlay5DisplayBg_ClearControl(0, 1);
    Overlay5DisplayBg_SetScreenSize(0, 1, 0);
    Overlay5DisplayBg_SetColorMode(0, 1, 0);
    Overlay5DisplayBg_SetScreenBase(0, 1, 0);
    Overlay5DisplayBg_SetCharacterBase(0, 1, 1);
    Overlay5DisplayBg_SetPriority(0, 1, 1);
    Overlay5DisplayBg_SetMosaic(0, 1, 0);
    Overlay5DisplayBg_ClearControl(0, 2);
    Overlay5DisplayBg_SetScreenSize(0, 2, 0);
    Overlay5DisplayBg_SetColorMode(0, 2, 0);
    Overlay5DisplayBg_SetScreenBase(0, 2, 1);
    Overlay5DisplayBg_SetCharacterBase(0, 2, 2);
    Overlay5DisplayBg_SetPriority(0, 2, 0);
    Overlay5DisplayBg_SetMosaic(0, 2, 0);
    func_ov005_02066638(128);
    func_ov005_020690f0(256);
    func_ov005_02069b54();
    func_ov005_02068ca8(64);
    func_ov005_02069674(64);
    func_ov005_02069998(32);
    archive = GameHeap_New(sizeof(Overlay5Archive), 0, 0, 1);
    if (archive)
        archive = func_ov005_02067584(archive, 10, 0, 0, -1);
    MENU_ARCHIVE = archive;
    DisplayVram_ClearEngineAssignments(0);
    DisplayVram_ClearEngineAssignments(1);
    MENU_OAM_BUFFERS = GameHeap_NewArray(17408, 0, 0, 0);
    func_02009078(0, MENU_OAM_BUFFERS, MENU_OAM_BUFFERS + 8192);
    func_02009078(1, MENU_OAM_BUFFERS + 8704, MENU_OAM_BUFFERS + 16896);
    irq = GameHeap_New(40, 0, 0, 0);
    if (irq)
        irq = EraseSceneIrq_Init(irq, 8, 0, task);
    task->irq = irq;
    GameDisplay_SetBackgroundColor(0, 0);
    REG32(0x04000000) &= ~0x1F00;
    REG32(0x04001000) &= ~0x1F00;
    GameIrqTask_Enable(task->irq);
    GX_DispOn();
    REG32(0x04001000) |= 0x10000;
    EraseScene_LoadResources(task);
    func_ov005_0206659c(func_ov008_02076c70, 6, 1);
    task->phase = 0;
    return task;
}

SaveSceneTask *EraseScene_Destroy(SaveSceneTask *task)
{
    task->base.vtable = &data_ov008_02078264;
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    {
        SaveMenuText *text = data_ov008_0207828c;
        if (text) {
            SaveMenuText_DeleteOwned(text);
            data_ov008_0207828c = 0;
        }
    }
    func_ov005_02065f10();
    func_02009058(0);
    func_02009058(1);
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
    data_02059f44 = 0;
    data_02059f48 = 0;
    GameTask_DestroyBase(&task->base);
    return task;
}

SaveSceneTask *EraseScene_Delete(SaveSceneTask *task)
{
    task->base.vtable = &data_ov008_02078264;
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    {
        SaveMenuText *text = data_ov008_0207828c;
        if (text) {
            SaveMenuText_DeleteOwned(text);
            data_ov008_0207828c = 0;
        }
    }
    func_ov005_02065f10();
    func_02009058(0);
    func_02009058(1);
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
    data_02059f44 = 0;
    data_02059f48 = 0;
    GameTask_DestroyBase(&task->base);
    GameHeap_Delete(task);
    return task;
}
