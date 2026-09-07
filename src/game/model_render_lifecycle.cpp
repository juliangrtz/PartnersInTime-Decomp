#include <game/battle_scene.h>
extern "C" {
#include <game/heap.h>
extern BattleModelVTable data_02050bb0;
extern BattleModel *BattleModelController_Init(BattleModel *);
extern BattleModel *BattleModelController_DestroyBase(BattleModel *);
extern void func_0202cbd4(void *, int, u32);
extern void MTX_Identity44_(MtxFx44 *);

BattleModel *BattleRenderModel_Init(BattleModel *model)
{
    BattleModelController_Init(model);
    *(BattleModelVTable **)model = &data_02050bb0;
    func_0202cbd4(&model->unknown_130, 0, model->unk_1b4 - (u8 *)&model->unknown_130);
    model->animation_state_bits.state = 31;
    model->owner_render_state = 0x7fff;
    model->unknown_16c = 16;
    model->unknown_16e = 1;
    MTX_Identity44_(&model->transform);
    return model;
}

BattleModel *BattleRenderModel_Destroy(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050bb0;
    model->unknown_c0();
    BattleModelController_DestroyBase(model);
    return model;
}

BattleModel *BattleRenderModel_Delete(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050bb0;
    model->unknown_c0();
    BattleModelController_DestroyBase(model);
    GameHeap_Delete(model);
    return model;
}

void BattleRenderModel_InitDescriptor(ModelRenderDescriptor *descriptor, int mode)
{
    BattleModel_InitDescriptor(descriptor);
    /* The original retains identical initialization paths for both modes. */
    if (!mode) {
        descriptor->flags &= ~0x380000;
        descriptor->unknown_58 = 0;
        descriptor->unknown_59 = 15;
        descriptor->unknown_5c = 0;
        return;
    } else {
        descriptor->flags &= ~0x380000;
        descriptor->unknown_58 = 0;
        descriptor->unknown_59 = 15;
        descriptor->unknown_5c = 0;
    }
}
}
