#include "save_menu_internal.h"

SaveSceneTask *SaveScene_Init(SaveSceneTask *task, u32 priority, u32 unused)
{

    Overlay5Archive *archive;
    GameIrqTask *irq;
    GameTask_Init(&task->base, priority, unused, 0);
    task->base.vtable = &data_ov008_02077f24;
    data_ov008_02078280 = task;
    func_ov005_02066128(0);
    data_0206032c[8] = 12;
    data_0206032c[9] = 3;
    Overlay5Display_ResetEngine(0);
    Overlay5Display_ResetEngine(1);
    func_ov005_0206781c(2, -16);
    REG16(0x4000304) &= ~0x8000u;
    func_02035fd0(16);
    func_0203613c(2);
    func_02035ebc(64);
    func_02035c00(1);
    func_02035b0c(32);
    func_020359c4(8);
    func_02035a40(4);
    func_02035938(128);
    GX_SetGraphicsMode(1, 3, 1);
    GXS_SetGraphicsMode(5);
    Overlay5Display_SetObjTileMappingMode(0, 1048592);
    Overlay5Display_SetObjTileMappingMode(1, 2097168);
    SaveScene_InitGeometry(task);
    Overlay5DisplayBg_SetPriority(0, 0, 2);
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
    Overlay5DisplayBg_ClearControl(0, 3);
    Overlay5DisplayBg_SetScreenSize(0, 3, 1);
    Overlay5DisplayBg_SetModeBits(0, 3, 0);
    Overlay5DisplayBg_SetScreenBase(0, 3, 2);
    Overlay5DisplayBg_SetCharacterBase(0, 3, 4);
    Overlay5DisplayBg_SetPriority(0, 3, 3);
    Overlay5DisplayBg_SetMosaic(0, 3, 0);
    Overlay5DisplayBg_ClearControl(1, 1);
    Overlay5DisplayBg_SetScreenSize(1, 1, 0);
    Overlay5DisplayBg_SetColorMode(1, 1, 1);
    Overlay5DisplayBg_SetScreenBase(1, 1, 0);
    Overlay5DisplayBg_SetCharacterBase(1, 1, 1);
    Overlay5DisplayBg_SetPriority(1, 1, 3);
    Overlay5DisplayBg_SetAreaOverflow(1, 1, 1);
    Overlay5DisplayBg_SetMosaic(1, 1, 0);
    Overlay5DisplayBg_ClearControl(1, 2);
    Overlay5DisplayBg_SetScreenSize(1, 2, 1);
    Overlay5DisplayBg_SetModeBits(1, 2, 0);
    Overlay5DisplayBg_SetScreenBase(1, 2, 1);
    Overlay5DisplayBg_SetCharacterBase(1, 2, 4);
    Overlay5DisplayBg_SetPriority(1, 2, 1);
    Overlay5DisplayBg_SetMosaic(1, 2, 0);
    Overlay5DisplayBg_ClearControl(1, 3);
    Overlay5DisplayBg_SetScreenSize(1, 3, 1);
    Overlay5DisplayBg_SetModeBits(1, 3, 0);
    Overlay5DisplayBg_SetScreenBase(1, 3, 2);
    Overlay5DisplayBg_SetCharacterBase(1, 3, 6);
    Overlay5DisplayBg_SetPriority(1, 3, 2);
    Overlay5DisplayBg_SetMosaic(1, 3, 0);
    func_ov005_02066638(256);
    func_ov005_020690f0(256);
    func_ov005_02069b54();
    func_ov005_02068ca8(64);
    func_ov005_02069674(256);
    func_ov005_02069998(3);
    archive = GameHeap_New(sizeof(Overlay5Archive), 0, 0, 1);
    if (archive)
        archive = func_ov005_02067584(archive, 10, 0, 0, -1);
    MENU_ARCHIVE = archive;
    DisplayVram_ClearEngineAssignments(0);
    DisplayVram_ClearEngineAssignments(1);
    MENU_OAM_BUFFERS = GameHeap_NewArray(17408, 0, 0, 0);
    func_02009078(0, MENU_OAM_BUFFERS, MENU_OAM_BUFFERS + 0x2000);
    func_02009078(1, MENU_OAM_BUFFERS + 8704, MENU_OAM_BUFFERS + 16896);
    irq = GameHeap_New(40, 0, 0, 0);
    if (irq)
        irq = SaveSceneIrq_Init(irq, 8, 0, task);
    task->irq = irq;
    GameDisplay_SetBackgroundColor(0x7FFF, 0x7FFF);
    REG16(0x4000050) = 0;
    REG16(0x4001050) = 0;
    REG32(0x4000000) = REG32(0x4000000) & 0xFFFFE0FF | 0x1900;
    REG32(0x4001000) = REG32(0x4001000) & 0xFFFFE0FF | 0x1200;
    GameIrqTask_Enable(task->irq);
    GX_DispOn();
    REG32(0x4001000) |= 0x10000u;
    func_ov008_0206c474(task);
    func_02036988((vu16 *)0x04000050, 1, 12, 16, 16);
    func_ov005_0206659c(func_ov008_0206bddc, 0, 1);
    task->phase = 0;
    return task;
}

SaveSceneTask *SaveScene_Destroy(SaveSceneTask *task)
{
    int i;
    task->base.vtable = &data_ov008_02077f24;
    func_ov005_020671e4(MENU_ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    for (i = 0; i < 4; ++i)
        func_ov005_02067424(MENU_ARCHIVE, (u8)i);
    func_ov008_0207275c();
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
    REG32(0x04000000) &= ~0x1F00;
    REG32(0x04001000) &= ~0x1F00;
    data_ov008_02078280 = 0;
    GameTask_DestroyBase(&task->base);
    return task;
}

SaveSceneTask *SaveScene_Delete(SaveSceneTask *task)
{
    int i;
    task->base.vtable = &data_ov008_02077f24;
    func_ov005_020671e4(MENU_ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    for (i = 0; i < 4; ++i)
        func_ov005_02067424(MENU_ARCHIVE, (u8)i);
    func_ov008_0207275c();
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
    REG32(0x04000000) &= ~0x1F00;
    REG32(0x04001000) &= ~0x1F00;
    data_ov008_02078280 = 0;
    GameTask_DestroyBase(&task->base);
    GameHeap_Delete(task);
    return task;
}
