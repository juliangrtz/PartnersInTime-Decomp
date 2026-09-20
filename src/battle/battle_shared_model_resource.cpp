/* Dispatch resource copy mode, shared alternate-model creation and sprite release. */
#include <game/battle_scene.h>
#include <game/battle_object.h>
#include <game/battle_ai.h>
#include <game/battle_model_animation.h>
extern "C" {
#include <game/heap.h>
extern u8 *data_ov002_020c0660;
void func_ov002_0206f3cc(BattleModel *, BattleObjectDataLoadState *, GameSpritePalette *, int);
void func_020093b4(BattleModel *, int);
}
#define SHARED_MODEL (*(BattleModel **)(data_ov002_020c0660 + 70444))

extern "C" void BattleObjectData_ControlResources(BattleObjectDataLoadState *resource, int operation, int copy)
{
    switch (operation) {
    case BATTLE_RESOURCE_SET_COPY_MODE:
        resource->flags.bits.copy_flag = copy;
        break;
    case BATTLE_RESOURCE_CREATE_SHARED_MODEL: {
        BattleModel *model = (BattleModel *)GameHeap_New(304, *(int *)(gBattleContext + 0xe15c), 0, 0);
        if (model) model = BattleModelController_InitBase(model);
        SHARED_MODEL = model;
        func_ov002_0206f3cc(SHARED_MODEL, resource, (GameSpritePalette *)(data_ov002_020c0660 + 70448), resource->resource_id);
        func_020093b4(SHARED_MODEL, 0);
        BattleModelAnimation_SetModels(0, SHARED_MODEL, (BattleModel *)-1, (BattleModel *)-1);
        break;
    }
    case BATTLE_RESOURCE_RELEASE_SPRITES:
        BattleObjectData_ReleaseSprites(resource);
        break;
    }
}
