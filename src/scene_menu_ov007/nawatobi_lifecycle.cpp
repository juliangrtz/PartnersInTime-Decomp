#include "nawatobi_internal.h"

extern "C" {
GameIrqTask *NawatobiSceneIrq_Init(GameIrqTask *task, u32 priority, u32 unused, void *parent)
{
    GameIrqTask_Init(task, priority, unused, parent);
    task->vtable = &data_ov007_0208e0bc;
    return task;
}

void NawatobiScene_UploadFrame(void)
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
        func_0203b7b4(data_ov007_020a6bc8 + 53248, Overlay5DisplayBg_GetScreenVram16K(DISPLAY_ENGINE_MAIN, 2),
                      49152);
        func_0203b7b4(data_ov007_020a6bc8, Overlay5DisplayBg_GetScreenVram16K(DISPLAY_ENGINE_SUB, 2), 49152);
        GameConsole_Upload(CONSOLE(0));
        GameConsole_Upload(CONSOLE(1));
        func_ov005_02065eec();
    }
}

NawatobiSceneTask *NawatobiScene_Init(NawatobiSceneTask *task, u32 priority, u32 unused)
{
    GameTask_Init(&task->task, priority, unused, 0);
    task->task.vtable = &data_ov007_0208e0d0;
    data_ov007_020a6bbc = task;
    func_ov005_02066128(1);
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_MAIN);
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_SUB);
    REG16(0x04000304) &= ~0x8000;
    func_ov005_0206781c(2, -16);
    func_02035fd0(1);
    func_0203613c(2);
    func_02035e04(32);
    func_020359c4(8);
    func_02035a40(4);
    func_020358ac(256);
    GX_SetGraphicsMode(1, 5, 0);
    GXS_SetGraphicsMode(5);
    Overlay5Display_SetObjTileMappingMode(DISPLAY_ENGINE_MAIN, DISPLAY_OBJ_TILE_MAPPING_1D_64K);
    Overlay5Display_SetObjTileMappingMode(DISPLAY_ENGINE_SUB, DISPLAY_OBJ_TILE_MAPPING_1D_64K);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_MAIN, 2);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_MAIN, 2, 1);
    Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_MAIN, 2, 128);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 4);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_MAIN, 2, 0);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_MAIN, 2, 2);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 2);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 2, 1);
    Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 2, 128);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 2, 4);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 2, 0);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 2);
    func_ov005_02066638(220);
    func_ov005_020690f0(256);
    func_ov005_02069b54();
    func_ov005_02068ca8(32);
    func_ov005_02069674(256);
    func_ov005_02069998(116);
    GameConsole *console = (GameConsole *)GameHeap_New(1604, 0, 0, 0);
    if (console)
        console = GameConsole_Init(console, 0, 0, 1, 0, 15, 7);
    CONSOLE(0) = console;
    console = (GameConsole *)GameHeap_New(1604, 0, 0, 0);
    if (console)
        console = GameConsole_Init(console, 1, 0, 1, 0, 15, 7);
    CONSOLE(1) = console;
    GameConsole_Clear(CONSOLE(0));
    GameConsole_Clear(CONSOLE(1));
    Overlay5Archive *archive = (Overlay5Archive *)GameHeap_New(13148, 0, 0, 1);
    if (archive)
        archive = func_ov005_02067584(archive, 10, 0, 0, -1);
    ARCHIVE = archive;
    DisplayVram_ClearEngineAssignments(0);
    DisplayVram_ClearEngineAssignments(1);
    DISPLAY.oam = GameHeap_NewArray(17408, 0, 0, 0);
    func_02009078(0, OAM_BUFFERS, OAM_BUFFERS + 8192);
    func_02009078(1, OAM_BUFFERS + 8704, OAM_BUFFERS + 16896);
    GameIrqTask *irq = (GameIrqTask *)GameHeap_New(40, 0, 0, 0);
    if (irq)
        irq = NawatobiSceneIrq_Init(irq, 8, 0, task);
    task->irq = irq;
    GameDisplay_SetBackgroundColor(0, 0);
    REG16(0x04000050) = 0;
    REG16(0x04001050) = 0;
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0x1500;
    REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x1500;
    GX_DispOn();
    REG32(0x04001000) |= 0x10000;
    MI_CpuFill8(data_ov007_020a6bcc, 0, 1680);
    NawatobiScene_LoadResources(task);
    void *manager = GameHeap_New(43056, 0, 0, 0);
    if (manager)
        SceneManager_LoadResources(manager);
    func_ov007_0208b420();
    SceneScript_LoadPrimaryArchive(data_ov007_020a6b90, 1, 1);
    func_ov005_0206659c(func_ov007_0208c5bc, 1, 1);
    task->phase = 0;
    ConsolePalette palette = data_ov007_0208e14c;
    Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_MAIN, &palette, 480, 32);
    Overlay5Display_LoadBgPalette(DISPLAY_ENGINE_SUB, &palette, 480, 32);
    GameIrqTask_Enable(task->irq);
    return task;
}

NawatobiSceneTask *NawatobiScene_Destroy(NawatobiSceneTask *task)
{
    task->task.vtable = &data_ov007_0208e0d0;
    func_ov005_020671e4(ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    {
        void *manager = data_ov007_020a6b90;
        if (manager) {
            DeleteSceneManager(manager);
            data_ov007_020a6b90 = 0;
        }
    }
    func_ov005_02067424(ARCHIVE, 2);
    func_ov005_02067424(ARCHIVE, 1);
    func_ov005_02065f10();
    if (data_ov007_020a6bc8) {
        GameHeap_DeleteArray(data_ov007_020a6bc8);
        data_ov007_020a6bc8 = 0;
    }
    if (data_ov007_020a6bc0) {
        GameHeap_DeleteArray(data_ov007_020a6bc0);
        data_ov007_020a6bc0 = 0;
    }
    if (data_ov007_020a6bc4) {
        GameHeap_DeleteArray(data_ov007_020a6bc4);
        data_ov007_020a6bc4 = 0;
    }
    func_02009058(0);
    func_02009058(1);
    func_ov005_02069928();
    func_ov005_02069660();
    func_ov005_02068c94();
    func_ov005_020699f0();
    func_ov005_020690dc();
    func_ov005_02066624();
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_MAIN);
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_SUB);
    func_ov005_0206781c(2, 0);
    data_ov007_020a6bbc = 0;
    GameTask_DestroyBase(&task->task);
    return task;
}

NawatobiSceneTask *NawatobiScene_Delete(NawatobiSceneTask *task)
{
    task->task.vtable = &data_ov007_0208e0d0;
    func_ov005_020671e4(ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    {
        void *manager = data_ov007_020a6b90;
        if (manager) {
            DeleteSceneManager(manager);
            data_ov007_020a6b90 = 0;
        }
    }
    func_ov005_02067424(ARCHIVE, 2);
    func_ov005_02067424(ARCHIVE, 1);
    func_ov005_02065f10();
    if (data_ov007_020a6bc8) {
        GameHeap_DeleteArray(data_ov007_020a6bc8);
        data_ov007_020a6bc8 = 0;
    }
    if (data_ov007_020a6bc0) {
        GameHeap_DeleteArray(data_ov007_020a6bc0);
        data_ov007_020a6bc0 = 0;
    }
    if (data_ov007_020a6bc4) {
        GameHeap_DeleteArray(data_ov007_020a6bc4);
        data_ov007_020a6bc4 = 0;
    }
    func_02009058(0);
    func_02009058(1);
    func_ov005_02069928();
    func_ov005_02069660();
    func_ov005_02068c94();
    func_ov005_020699f0();
    func_ov005_020690dc();
    func_ov005_02066624();
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_MAIN);
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_SUB);
    func_ov005_0206781c(2, 0);
    data_ov007_020a6bbc = 0;
    GameTask_DestroyBase(&task->task);
    GameHeap_Delete(task);
    return task;
}
}
