#include <game/battle_context.h>
#include <game/battle_display_capture.h>
#include <game/battle_object.h>

enum BattleDisplayCaptureOffset {
    BATTLE_DISPLAY_MODE_OFFSET = 0x454,
    BATTLE_DISPLAY_STATE_OFFSET = 0xD3A0
};

enum BattleDisplayCaptureConstant {
    BATTLE_DISPLAY_STATE_CAPTURE_ACTIVE = 1 << 5,
    BATTLE_DISPLAY_MODE_RESTORE = 1,
    BATTLE_DISPLAY_MODE_FINISH = 3,
    BATTLE_CAPTURE_CONTROL_BASE = 0x80351010,
    REG_DISPCAPCNT_ADDRESS = 0x04000064
};

extern u8 *gBattleSystem;

void *func_ov002_02072508(
    int (*callback)(BattleObjectUploadTask *task),
    BattleSceneResource *argument, int unknown_2, int unknown_3);
void *func_ov002_020725a4(
    int (*callback)(BattleObjectUploadTask *task),
    BattleSceneResource *argument, int unknown_2, int unknown_3);
BattleQueuedTask *BattleCaptureSurface_QueueUpload(
    u32 vram_address, int object_data_id,
    int x, int y, int width, int height);

void BattleDisplayCapture_FinishResetTask(BattleObjectUploadTask *task);
int BattleDisplayCapture_QueueFinishResetTask(BattleObjectUploadTask *task);
int BattleDisplayCapture_RestoreResetModeTask(BattleObjectUploadTask *task);
void BattleDisplayCapture_FinishConfigureTask(BattleObjectUploadTask *task);
int BattleDisplayCapture_WriteControlTask(BattleObjectUploadTask *task);
int BattleDisplayCapture_RestoreConfigureModeTask(
    BattleObjectUploadTask *task);

/* Metrowerks emits C functions in reverse source order. */
int BattleDisplayCapture_QueueConfigure(int capture_source) {
    *(u32 *)(gBattleContext + BATTLE_DISPLAY_STATE_OFFSET) |=
        BATTLE_DISPLAY_STATE_CAPTURE_ACTIVE;
    return (int)func_ov002_020725a4(
        BattleDisplayCapture_RestoreConfigureModeTask,
        (BattleSceneResource *)capture_source, 0, 0);
}

int BattleDisplayCapture_RestoreConfigureModeTask(
    BattleObjectUploadTask *task) {
    *(u32 *)(gBattleSystem + BATTLE_DISPLAY_MODE_OFFSET) =
        BATTLE_DISPLAY_MODE_RESTORE;
    return (int)func_ov002_02072508(
        BattleDisplayCapture_WriteControlTask, task->resource, 0, 0);
}

int BattleDisplayCapture_WriteControlTask(BattleObjectUploadTask *task) {
    *(volatile u32 *)REG_DISPCAPCNT_ADDRESS =
        BATTLE_CAPTURE_CONTROL_BASE | ((u32)task->resource << 24);
    return (int)func_ov002_020725a4(
        (int (*)(BattleObjectUploadTask *))
            BattleDisplayCapture_FinishConfigureTask,
        0, 0, 0);
}

void BattleDisplayCapture_FinishConfigureTask(
    BattleObjectUploadTask *task) {
    *(u32 *)(gBattleSystem + BATTLE_DISPLAY_MODE_OFFSET) =
        BATTLE_DISPLAY_MODE_FINISH;
    *(u32 *)(gBattleContext + BATTLE_DISPLAY_STATE_OFFSET) &=
        ~BATTLE_DISPLAY_STATE_CAPTURE_ACTIVE;
}

int BattleDisplayCapture_QueueReset(void) {
    *(u32 *)(gBattleContext + BATTLE_DISPLAY_STATE_OFFSET) |=
        BATTLE_DISPLAY_STATE_CAPTURE_ACTIVE;
    return (int)func_ov002_020725a4(
        BattleDisplayCapture_RestoreResetModeTask, 0, 0, 0);
}

int BattleDisplayCapture_RestoreResetModeTask(
    BattleObjectUploadTask *task) {
    *(u32 *)(gBattleSystem + BATTLE_DISPLAY_MODE_OFFSET) =
        BATTLE_DISPLAY_MODE_RESTORE;
    return (int)func_ov002_02072508(
        BattleDisplayCapture_QueueFinishResetTask, 0, 0, 0);
}

int BattleDisplayCapture_QueueFinishResetTask(
    BattleObjectUploadTask *task) {
    return (int)func_ov002_020725a4(
        (int (*)(BattleObjectUploadTask *))
            BattleDisplayCapture_FinishResetTask,
        0, 0, 0);
}

void BattleDisplayCapture_FinishResetTask(BattleObjectUploadTask *task) {
    *(u32 *)(gBattleSystem + BATTLE_DISPLAY_MODE_OFFSET) =
        BATTLE_DISPLAY_MODE_FINISH;
    *(u32 *)(gBattleContext + BATTLE_DISPLAY_STATE_OFFSET) &=
        ~BATTLE_DISPLAY_STATE_CAPTURE_ACTIVE;
}

int BattleObjectData_QueueCaptureSurfaceUpload(
    BattleObjectDataLoadState *load_state) {
    return (int)BattleCaptureSurface_QueueUpload(
        0x38000, load_state->object_data_id, 0, 80, 256, 128);
}
