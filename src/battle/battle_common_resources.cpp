extern "C" {
#include <game/heap.h>
}
#include <game/battle_common_resources.h>
#include <game/battle_reward_items.h>
#include "battle_dialogue_control_internal.h"
#include <game/battle_scene.h>
extern "C" {
#include <game/sprite_output.h>
void func_ov002_0206cc68(void);
void func_02009094(int);
}

struct CommonFrameTask {
    int unknown_00, unknown_04;
    void (*update)(CommonFrameTask *);
};
struct CommonFrameWork {
    u8 unknown_000[428];
    void (*prepare_oam)(void);
    u8 unknown_1b0[70484 - 432];
    CommonFrameTask tasks[4];
};
typedef char CommonFrameTaskSizeCheck[sizeof(CommonFrameTask) == 12 ? 1 : -1];
typedef char CommonFrameViewBoundCheck[sizeof(CommonFrameWork) + 3 * 12 <= 70976 ? 1 : -1];
#define WORK ((u8 *)data_ov002_020c0660)
#define AUXILIARY_MODEL (*(BattleModel **)(WORK + 70444))

static inline void DeleteAuxiliaryModel(BattleModel *model) {
    if (model)
        model->unknown_14();
}

extern "C" {
void BattleCommonResources_OamReady(void) {}
void BattleCommonResources_ResetOam(void) {
    GameOam_SetReserved(1, 0, 0, 0);
    ((CommonFrameWork *)data_ov002_020c0660)->prepare_oam = BattleCommonResources_OamReady;
}
void BattleCommonResources_PrepareOam(void) {
    ((CommonFrameWork *)data_ov002_020c0660)->prepare_oam();
}
}

extern "C" {
#include <game/battle_attack_loader.h>
#include <game/battle_scene.h>
#include <game/heap.h>
#include <game/sprite_output.h>
#include <game/model_animation.h>
#include <game/rumble.h>
extern int data_ov002_020c071c;
extern const u8 data_0205674c[];
extern const GameRumblePattern data_ov002_020be50c[];
void func_0202cbd4(void *, int, u32);
int func_02007ebc(GameSpritePalette *, int, int, int, int, int, int, int, const void *, u32);
void func_ov002_0206bf3c(FieldRenderObject *, FieldRenderObject *, GameModelAnimationContext *,
                         MtxFx44 *);
void func_ov002_02066bc4(void);
}

enum {
    WORKSPACE_BYTES = 70976,
    SPRITE_ALLOCATION = 384,
    SPRITE_PALETTE = 408,
    GRAPHICS_BUFFER = 508,
    MODEL_ANIMATION_POOL = 53780,
    MATRIX_ANIMATION_POOL = 53784,
    MATRIX_STORAGE = 53788,
    MODEL_STORAGE = 69148
};
/* The workspace also owns the overlay loader and dialogue state. These offsets
   address its shared graphics and animation storage. */

extern "C" void BattleCommonResources_Initialize(void *unused_context) {
    if (!data_ov002_020c0660) {
        data_ov002_020c0660 =
            (BattleOverlayLoadState *)GameHeap_New(WORKSPACE_BYTES, data_ov002_020c071c, 0, 0);
        func_0202cbd4(data_ov002_020c0660, 0, WORKSPACE_BYTES);
        *(void **)(WORK + GRAPHICS_BUFFER) = GameHeap_NewArray(8192, data_ov002_020c071c, 0, 0);
    }
    GameSpriteAllocation_Allocate((GameSpriteAllocation *)(WORK + SPRITE_ALLOCATION), 1, 1, 64, 1,
                                  0xc2ffffff, 1, 0);
    func_02007ebc((GameSpritePalette *)(WORK + SPRITE_PALETTE), 1, 0, 16, 15, 1, 1, 1,
                  data_0205674c, 0xc2ffffff);
    GameMatrixAnimation *matrices =
        (GameMatrixAnimation *)GameHeap_New(sizeof(GameMatrixAnimation), data_ov002_020c071c, 0, 0);
    if (matrices)
        matrices = GameMatrixAnimation_Initialize(matrices, WORK + MATRIX_STORAGE, 32, 32);
    *(GameMatrixAnimation **)(WORK + MATRIX_ANIMATION_POOL) = matrices;
    /* The native allocation reserves four bytes beyond the 24-byte pool. */
    GameModelAnimation *models = (GameModelAnimation *)GameHeap_New(28, data_ov002_020c071c, 0, 0);
    if (models)
        models = GameModelAnimation_Initialize(
            models, *(GameMatrixAnimation **)(WORK + MATRIX_ANIMATION_POOL),
            (GameModelAnimationContext *)(WORK + MODEL_STORAGE), 32, func_ov002_0206bf3c);
    *(GameModelAnimation **)(WORK + MODEL_ANIMATION_POOL) = models;
    func_ov002_02066bc4();
    GameRumble_SetPatterns(data_ov002_020be50c, 8);
}

struct BattleCommonLoadFlags {
    u32 phase : 3, unknown03 : 29;
};
#define LOAD_STATE (*(BattleCommonLoadFlags *)(WORK + 53776))
extern "C" {
void func_ov002_02069530(int, int);
void func_ov002_020691b8(BattleSceneObject *, int, int, int);
void func_ov002_02068b50(BattleSceneObject *, int, int, int, int);
void func_ov002_02068878(BattleObjectDataLoadState *, int, int);
int BattleCommonResources_UpdateLoad(void) {
    switch (LOAD_STATE.phase) {
    case 0:
        BattleObjectData_AllocateLoadBuffer(20, 4096);
        BattleObjectData_GetLoadState(20)->flags.bits.copy_flag = 0;
        BattleObjectData_EnsureLoaded(20, (s32)0xc2000012);
        LOAD_STATE.phase = 1;
        break;
    case 1:
        if (!BattleObjectData_IsLoadPending(20))
            LOAD_STATE.phase = 2;
        break;
    case 2: {
        func_ov002_02069530(20, 0);
        BattleSceneObject *first = BattleSceneObject_GetById(17);
        BattleEntity_BindResource(17, 20);
        BattleSceneObject_SetAnimation(BattleSceneObject_GetById(17), 1, 1);
        BattleSceneObject *second = BattleSceneObject_GetById(16);
        BattleEntity_BindResource(16, 20);
        func_ov002_020691b8(second, 5, 0, 0);
        func_ov002_02068b50(second, 4, 0, 0, 0);
        func_ov002_02068b50(second, 5, 2, 0, 0);
        func_ov002_02068b50(second, 14, 1, 0, 0);
        BattleSceneObject_AdjustPosition(first, -128 - first->x, -256 - first->y, -first->z);
        BattleSceneObject_AdjustPosition(second, -128 - second->x, -256 - second->y, -second->z);
        func_ov002_02068878(BattleObjectData_GetLoadState(17), 1, 0);
        LOAD_STATE.phase = 3;
    }
    /* Fall through once both reserved objects have been prepared. */
    case 3:
        return 1;
    }
    return 0;
}
}

extern "C" void BattleCommonResources_ReleaseGraphics(void *unused_context) {
    func_ov002_0206cc68();
    if (AUXILIARY_MODEL) {
        GameSpritePalette_Unlink((GameSpritePalette *)(WORK + 70448));
        AUXILIARY_MODEL->stop();
        if (AUXILIARY_MODEL) {
            DeleteAuxiliaryModel(AUXILIARY_MODEL);
            AUXILIARY_MODEL = 0;
        }
    }
    BattleDialogue_Destroy();
    DisplayVram_ClearEngineAssignments(0);
    DisplayVram_ClearEngineAssignments(1);
    func_02009094(1);
}

extern "C" void BattleCommonResources_Update(void *unused_context) {
    ((CommonFrameWork *)WORK)->prepare_oam = BattleCommonResources_ResetOam;
    if (DialogueController())
        DialogueController()->unknown_08();
    for (int i = 0; i < 4; ++i) {
        /* Apply the slot stride before the fixed callback offset, as in native code.
           The indexed view remains inside the shared allocation. */
        CommonFrameWork *indexed = (CommonFrameWork *)(WORK + i * 12);
        if (indexed->tasks[0].update)
            indexed->tasks[0].update((CommonFrameTask *)(WORK + 70484 + i * 12));
    }
    BattleRewardItems_Update();
}
