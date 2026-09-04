#include <game/battle_background.h>
#include <game/save_data.h>

enum BattleBackgroundConstant {
    SAVE_BATTLE_BACKGROUND_FLAGS_OFFSET = 0x514,
    SAVE_CURRENT_MAP_ID_OFFSET = 0x55A,
    BATTLE_BACKGROUND_DEFAULT_ID = 4,
    BATTLE_BACKGROUND_LOAD_DURATION = 0x800
};

typedef union SaveBattleBackgroundFlags {
    u8 raw;
    struct {
        u8 change_disabled : 1;
        u8 unknown_01_07 : 7;
    } bits;
} SaveBattleBackgroundFlags;

void FieldAssets_LoadForMap(s16 map_id, int duration);
void SceneBackground_StartFade(int fade_out, int duration);
void SceneBackground_Set(int background_id);
void SceneBackground_Load(u16 background_id, u8 buffer_index, int duration);

BattleQueuedTask *BattleBackground_RequestLoad(void) {
    BattleRuntimeFlags *flags;

    if (((SaveBattleBackgroundFlags *)(
            gSaveData + SAVE_BATTLE_BACKGROUND_FLAGS_OFFSET))
            ->bits.change_disabled) {
        return 0;
    }

    flags = (BattleRuntimeFlags *)(gBattleContext +
                                   BATTLE_RUNTIME_FLAGS_OFFSET);
    flags->raw |= BATTLE_RUNTIME_FLAG_ASSET_LOAD_PENDING;
    return BattleTaskQueue_Enqueue(BattleBackground_LoadTask, 0);
}

void BattleBackground_LoadTask(BattleQueuedTask *task) {
    BattleRuntimeState *runtime = BattleContext_GetRuntimeState();
    u16 background_id = ((BattleContext *)gBattleContext)->background_id;

    if (background_id == 0) {
        background_id = BATTLE_BACKGROUND_DEFAULT_ID;
    }
    SceneBackground_Load(background_id, runtime->flags.bits.background_buffer,
                         BATTLE_BACKGROUND_LOAD_DURATION);
    task->callback = BattleBackground_FinishLoadTask;
}

void BattleBackground_FinishLoadTask(BattleQueuedTask *task) {
    BattleRuntimeFlags *flags = (BattleRuntimeFlags *)(
        gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET);

    flags->raw &= ~BATTLE_RUNTIME_FLAG_ASSET_LOAD_PENDING;
    task->callback = 0;
}

BattleQueuedTask *BattleBackground_RequestToggle(void) {
    if (((SaveBattleBackgroundFlags *)(
            gSaveData + SAVE_BATTLE_BACKGROUND_FLAGS_OFFSET))
            ->bits.change_disabled) {
        return 0;
    }

    return BattleTaskQueue_Enqueue(
        (BattleQueuedTaskCallback)BattleBackground_ToggleTask, 0);
}

int BattleBackground_ToggleTask(BattleQueuedTask *task) {
    int background_id;
    int result;

    SceneBackground_StartFade(0, 1);
    background_id = ((BattleContext *)gBattleContext)->background_id;
    if (background_id == 0) {
        background_id = BATTLE_BACKGROUND_DEFAULT_ID;
    }
    SceneBackground_Set(background_id);

    ((BattleContext *)gBattleContext)->runtime.flags.bits.background_buffer ^=
        1;
    result = (int)&((BattleContext *)gBattleContext)->runtime;
    ((BattleContext *)gBattleContext)->runtime.flags.raw &=
        ~BATTLE_RUNTIME_FLAG_BACKGROUND_FADED_OUT;
    task->callback = 0;
    return result;
}

BattleQueuedTask *BattleFieldAssets_RequestReload(void) {
    BattleRuntimeFlags *flags = (BattleRuntimeFlags *)(
        gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET);

    flags->raw |= BATTLE_RUNTIME_FLAG_ASSET_LOAD_PENDING;
    return BattleTaskQueue_Enqueue(BattleFieldAssets_ReloadTask, 0);
}

void BattleFieldAssets_ReloadTask(BattleQueuedTask *task) {
    s16 map_id = *(s16 *)(gSaveData + SAVE_CURRENT_MAP_ID_OFFSET);

    FieldAssets_LoadForMap(map_id, BATTLE_BACKGROUND_LOAD_DURATION);
    task->callback = BattleFieldAssets_FinishReloadTask;
}

void BattleFieldAssets_FinishReloadTask(BattleQueuedTask *task) {
    BattleRuntimeFlags *flags = (BattleRuntimeFlags *)(
        gBattleContext + BATTLE_RUNTIME_FLAGS_OFFSET);

    flags->raw &= ~BATTLE_RUNTIME_FLAG_ASSET_LOAD_PENDING;
    task->callback = 0;
}
