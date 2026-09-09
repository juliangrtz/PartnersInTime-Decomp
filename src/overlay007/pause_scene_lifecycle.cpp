#include "pause_scene_internal.h"

extern "C" {

PauseSceneTask *PauseScene_Init(PauseSceneTask *task, u32 priority, u32 unused)
{
    void *object;
    GameTask_Init(&task->task, priority, unused, 0);
    task->task.vtable = &data_ov007_0208d9b8;
    data_ov007_0208e1e0 = task;
    func_ov005_02066128(0);
    ((GameInput *)data_0206032c)->first_delay = 12;
    ((GameInput *)data_0206032c)->repeat_delay = 3;
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_MAIN);
    Overlay5Display_ResetEngine(DISPLAY_ENGINE_SUB);
    REG16(0x4000304) &= ~0x8000u;
    func_ov005_0206781c(2, -16);
    func_02035fd0(16);
    func_0203613c(3);
    func_02035e04(32);
    func_02035ebc(64);
    func_020359c4(8);
    func_02035a40(4);
    func_020358ac(256);
    func_02035938(128);
    GX_SetGraphicsMode(1, 5, 0);
    GXS_SetGraphicsMode(5);
    Overlay5Display_SetObjTileMappingMode(DISPLAY_ENGINE_MAIN, DISPLAY_OBJ_TILE_MAPPING_1D_64K);
    Overlay5Display_SetObjTileMappingMode(DISPLAY_ENGINE_SUB, DISPLAY_OBJ_TILE_MAPPING_1D_64K);
    func_ov007_02081664(0);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_MAIN, 1);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_MAIN, 1, 1);
    Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_MAIN, 1, 1);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 1, 1);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 1, 2);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_MAIN, 1, 0);
    Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_MAIN, 1, 1);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_MAIN, 1, 2);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_MAIN, 2);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_MAIN, 2, 1);
    Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_MAIN, 2, 0);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 2, 3);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 2, 4);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_MAIN, 2, 0);
    Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_MAIN, 2, 0);
    Overlay5DisplayBg_SetOverflow(DISPLAY_ENGINE_MAIN, 2, 0);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_MAIN, 2, 1);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_MAIN, 3);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_MAIN, 3, 1);
    Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_MAIN, 3, 0);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_MAIN, 3, 6);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_MAIN, 3, 8);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_MAIN, 3, 0);
    Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_MAIN, 3, 0);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_MAIN, 3, 3);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 0);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 0, 0);
    Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_SUB, 0, 0);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 0, 13);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 0, 0);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 0, 0);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 0, 0);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 1);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 1, 0);
    Overlay5DisplayBg_SetColorMode(DISPLAY_ENGINE_SUB, 1, 0);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 1, 14);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 1, 1);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 1, 1);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 1, 0);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 2);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 2, 1);
    Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 2, 128);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 2, 5);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 2, 2);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 2, 0);
    Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 2, 0);
    Overlay5DisplayBg_ClearControl(DISPLAY_ENGINE_SUB, 3);
    Overlay5DisplayBg_SetScreenSize(DISPLAY_ENGINE_SUB, 3, 1);
    Overlay5DisplayBg_SetModeBits(DISPLAY_ENGINE_SUB, 3, 0);
    Overlay5DisplayBg_SetScreenBase(DISPLAY_ENGINE_SUB, 3, 15);
    Overlay5DisplayBg_SetCharacterBase(DISPLAY_ENGINE_SUB, 3, 2);
    Overlay5DisplayBg_SetPriority(DISPLAY_ENGINE_SUB, 3, 3);
    Overlay5DisplayBg_SetMosaic(DISPLAY_ENGINE_SUB, 3, 0);
    Overlay5DisplayBg_SetAreaOverflow(DISPLAY_ENGINE_SUB, 3, 0);

    func_ov005_02066638(288);
    func_ov005_020690f0(288);
    func_ov005_02069b54();
    func_ov005_02068ca8(32);
    func_ov005_02069674(256);
    func_ov005_02069998(116);
    object = GameHeap_New(13148, 0, 0, 1);
    if (object)
        object = func_ov005_02067584((Overlay5Archive *)object, 10, 0, 0, -1);
    ARCHIVE = (Overlay5Archive *)object;
    DisplayVram_ClearEngineAssignments(DISPLAY_ENGINE_MAIN);
    DisplayVram_ClearEngineAssignments(DISPLAY_ENGINE_SUB);
    OAM_BUFFERS = (u8 *)GameHeap_NewArray(17408, 0, 0, 0);
    func_02009078(0, OAM_BUFFERS, OAM_BUFFERS + 8192);
    func_02009078(1, OAM_BUFFERS + 8704, OAM_BUFFERS + 16896);
    object = GameHeap_New(40, 0, 0, 0);
    if (object)
        object = PauseSceneIrq_Init((GameIrqTask *)object, 8, 0, task);
    task->irq = (GameIrqTask *)object;
    GameDisplay_SetBackgroundColor(0, 0);
    REG16(0x04000050) = 0;
    REG16(0x04001050) = 0;
    REG32(0x04000000) = (REG32(0x04000000) & ~0x1f00) | 0x1d00;
    REG32(0x04001000) = (REG32(0x04001000) & ~0x1f00) | 0x1d00;
    GX_DispOn();
    REG32(0x04001000) |= 0x10000;
    MI_CpuFill8(&WORK, 0, 90600);
    MI_CpuFill8(data_ov007_0208e1e8, 0, 9224);
    GameRumble_SetPatterns(data_ov007_0208d738, 8);
    PauseScene_LoadResources(task);
    task->menu = func_ov005_0206659c(func_ov007_0206e918, 1, 1);
    task->phase = 0;
    if (SAVED_TUTORIAL)
        WORK.tutorial = 1;
    else if (!VM_ReadVariable(8224, 0, 0) && VM_ReadVariable(*(u16 *)(data_ov007_0208e02c + 4), 0, 0)) {
        WORK.tutorial = 1;
        SAVED_TUTORIAL = 4;
        VM_WriteVariable(8224, 1, 0, 0);
    }
    object = GameHeap_New(43056, 0, 0, 0);
    if (object)
        SceneManager_LoadResources(object);
    SceneController_Create(data_ov007_020a6b90);
    SceneTask_LoadArchiveMember(data_ov007_020a6b90);
    if (WORK.tutorial) {
        SceneScript_LoadPrimaryArchive(data_ov007_020a6b90, 0, (u16)(SAVED_TUTORIAL - 1));
        WORK.input_mask = 65535;
    }
    func_ov007_0208a918();
    GameIrqTask_Enable(task->irq);
    return task;
}

PauseSceneTask *PauseScene_Destroy(PauseSceneTask *task)
{

    task->task.vtable = &data_ov007_0208d9b8;
    func_ov005_020671e4(ARCHIVE);
    if (task->irq) {
        GameIrqTask_DeleteSafe(task->irq);
        task->irq = 0;
    }
    MI_StopDma(2);
    if (data_ov007_020a6b90) {
        DeleteSceneManager(data_ov007_020a6b90);
        data_ov007_020a6b90 = 0;
    }
    for (int i = 0; i < 48; ++i)
        if (WORK.sprites[i]) {
            func_ov005_020695cc(WORK.sprites[i]);
            WORK.sprites[i] = 0;
        }
    if (WORK.renderer) {
        func_ov005_02068c54(WORK.renderer);
        WORK.renderer = 0;
    }
    if (WORK.palette_controller) {
        DeletePausePanel((PausePanel *)WORK.palette_controller);
        WORK.palette_controller = 0;
    }
    func_ov005_02067424(ARCHIVE, 2);
    func_ov005_02067424(ARCHIVE, 1);
    for (int j = 0; j < 2; ++j) {
        if (WORK.ownedc4[j]) {
            GameHeap_DeleteArray(WORK.ownedc4[j]);
            WORK.ownedc4[j] = 0;
        }
        if (WORK.ownedcc[j]) {
            GameHeap_DeleteArray(WORK.ownedcc[j]);
            WORK.ownedcc[j] = 0;
        }
        if (WORK.ownedd4[j]) {
            GameHeap_DeleteArray(WORK.ownedd4[j]);
            WORK.ownedd4[j] = 0;
        }
    }
    if (WORK.number_glyph_pixels) {
        GameHeap_DeleteArray(WORK.number_glyph_pixels);
        WORK.number_glyph_pixels = 0;
    }
    if (WORK.number_glyph_tiles) {
        GameHeap_DeleteArray(WORK.number_glyph_tiles);
        WORK.number_glyph_tiles = 0;
    }
    if (WORK.ownedc0) {
        GameHeap_DeleteArray(WORK.ownedc0);
        WORK.ownedc0 = 0;
    }
    if (WORK.owned94) {
        GameHeap_DeleteArray(WORK.owned94);
        WORK.owned94 = 0;
    }
    GameSpritePalette_Unlink(&WORK.palettee8);
    GameSpritePalette_Unlink(&WORK.palettefc);
    GameSpritePalette_Unlink(&WORK.palette98);
    GameSpritePalette_Unlink(&WORK.paletteac);
    if (WORK.owned90) {
        GameHeap_DeleteArray(WORK.owned90);
        WORK.owned90 = 0;
    }
    if (WORK.owned8c) {
        GameHeap_DeleteArray(WORK.owned8c);
        WORK.owned8c = 0;
    }
    if (WORK.owned88) {
        GameHeap_DeleteArray(WORK.owned88);
        WORK.owned88 = 0;
    }
    if (WORK.number_scratch) {
        GameHeap_DeleteArray(WORK.number_scratch);
        WORK.number_scratch = 0;
    }
    if (data_ov007_0208e1e4) {
        DeleteParty(data_ov007_0208e1e4);
        data_ov007_0208e1e4 = 0;
    }
    if (WORK.owned80) {
        GameHeap_DeleteArray(WORK.owned80);
        WORK.owned80 = 0;
    }
    if (WORK.owned28) {
        GameHeap_DeleteArray(WORK.owned28);
        WORK.owned28 = 0;
    }
    if (WORK.owned2c) {
        GameHeap_DeleteArray(WORK.owned2c);
        WORK.owned2c = 0;
    }
    GameSpriteAllocation_Unlink(&WORK.main_allocation);
    GameSpriteAllocation_Unlink(&WORK.sub_allocation);
    func_ov005_02065f10();
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
    if (data_0205a00c)
        GameRumble_Stop();
    data_ov007_0208e1e0 = 0;
    GameTask_DestroyBase(&task->task);
    return task;
}
}
