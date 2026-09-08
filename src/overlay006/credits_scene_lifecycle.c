#include "frontend_scene_internal.h"

CreditsSceneTask *CreditsScene_Init(CreditsSceneTask *task, u32 priority, u32 unused)
{

    Overlay5Archive *archive;
    GameIrqTask *irq;
    GameTask_Init(&task->base, priority, unused, 0);
    task->base.vtable = &data_ov006_0207bc1c;
    data_ov006_0207c4f0 = task;
    func_ov005_02066128(0);
    data_0206032c[8] = 12;
    data_0206032c[9] = 3;
    Overlay5Display_ResetEngine(0);
    Overlay5Display_ResetEngine(1);
    func_ov005_0206781c(2, -16);
    REG16(0x4000304) &= ~0x8000u;
    func_02035fd0(16);
    func_02035c00(1);
    func_02035b0c(64);
    func_02035a40(4);
    func_020359c4(8);
    func_02035aec(2);
    GX_SetGraphicsMode(1, 3, 1);
    GXS_SetGraphicsMode(3);
    Overlay5Display_SetObjTileMappingMode(0, 1048592);
    Overlay5Display_SetObjTileMappingMode(1, 1048592);
    CreditsScene_InitGeometry(task);
    Overlay5DisplayBg_SetPriority(0, 0, 3);
    REG16(0x400100E) = REG16(0x400100E) & 0x43 | 0x4084;
    Overlay5DisplayBg_SetPriority(1, 3, 3);
    func_ov005_02066638(512);
    func_ov005_020690f0(256);
    func_ov005_02069b54();
    func_ov005_02068ca8(32);
    func_ov005_02069674(294);
    func_ov005_02069998(1);
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
        irq = CreditsSceneIrq_Init(irq, 8, 0, task);
    task->irq = irq;
    GameDisplay_SetBackgroundColor(0, 0);
    REG16(0x4000050) = 0;
    REG16(0x4001050) = 0;
    REG32(0x4000000) = REG32(0x4000000) & 0xFFFFE0FF | 0x1100;
    REG32(0x4001000) = REG32(0x4001000) & 0xFFFFE0FF | 0x1800;
    GameIrqTask_Enable(task->irq);
    GX_DispOn();
    REG32(0x4001000) |= 0x10000u;
    CreditsScene_LoadResources(task);
    func_02036988((vu16 *)0x04000050, 1, 12, 16, 16);
    func_ov005_0206659c(func_ov006_02077d5c, 0, 1);
    task->phase = 0;
    return task;
}

CreditsSceneTask *CreditsScene_Destroy(CreditsSceneTask *task)
{
    int i;
    task->base.vtable = &data_ov006_0207bc1c;
    func_ov005_020671e4(MENU_ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    for (i = 0; i < 3; ++i)
        func_ov005_02067424(MENU_ARCHIVE, (u8)i);
    func_ov005_02065f10();
    if (task->font) {
        GameHeap_DeleteArray(task->font);
        task->font = 0;
    }
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
    data_ov006_0207c4f0 = 0;
    GameTask_DestroyBase(&task->base);
    return task;
}

CreditsSceneTask *CreditsScene_Delete(CreditsSceneTask *task)
{
    int i;
    task->base.vtable = &data_ov006_0207bc1c;
    func_ov005_020671e4(MENU_ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    for (i = 0; i < 3; ++i)
        func_ov005_02067424(MENU_ARCHIVE, (u8)i);
    func_ov005_02065f10();
    if (task->font) {
        GameHeap_DeleteArray(task->font);
        task->font = 0;
    }
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
    data_ov006_0207c4f0 = 0;
    GameTask_DestroyBase(&task->base);
    GameHeap_Delete(task);
    return task;
}
