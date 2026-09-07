#include <game/battle_scene.h>
extern "C" {
#include <game/heap.h>
extern BattleModelVTable data_02050a24;
extern void func_02009138(BattleModel *);

BattleModel *BattleModel_DestroyResource(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050a24;
    model->unknown_1c();
    func_02009138(model);
    return model;
}

BattleModel *BattleModel_DeleteResource(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050a24;
    model->unknown_1c();
    func_02009138(model);
    GameHeap_Delete(model);
    return model;
}

BattleModel *BattleModel_DestroyResourceBase(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050a24;
    model->unknown_1c();
    func_02009138(model);
    return model;
}

void BattleModel_UnlinkTexture(BattleModel *model)
{
    GameSpriteAllocation_Unlink(&model->texture);
}
}
