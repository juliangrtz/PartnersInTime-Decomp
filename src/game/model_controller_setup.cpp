#include <game/battle_scene.h>
#include <game/model_resource.h>
extern "C" {
#include <game/heap.h>
extern BattleModelVTable data_02050aa4;
extern void func_0202cbd4(void *, int, u32);
extern BattleModel *BattleModel_InitResourceState(BattleModel *);
extern BattleModel *BattleModel_DestroyResourceBase(BattleModel *);

BattleModel *BattleModelController_Init(BattleModel *model)
{
    BattleModel_InitResourceState(model);
    *(BattleModelVTable **)model = &data_02050aa4;
    func_0202cbd4(&model->animation_controller, 0,
        model->unk_12c - (u8 *)&model->animation_controller);
    return model;
}

BattleModel *BattleModelController_InitBase(BattleModel *model)
{
    BattleModel_InitResourceState(model);
    *(BattleModelVTable **)model = &data_02050aa4;
    func_0202cbd4(&model->animation_controller, 0,
        model->unk_12c - (u8 *)&model->animation_controller);
    return model;
}

BattleModel *BattleModelController_Destroy(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050aa4;
    BattleModel_DestroyResourceBase(model);
    return model;
}

BattleModel *BattleModelController_Delete(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050aa4;
    BattleModel_DestroyResourceBase(model);
    GameHeap_Delete(model);
    return model;
}

BattleModel *BattleModelController_DestroyBase(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_02050aa4;
    BattleModel_DestroyResourceBase(model);
    return model;
}

int BattleModelController_RestoreResources(BattleModel *model, const void *descriptor)
{
    return BattleModel_InitializeSpriteResources(model, (const ModelResourceDescriptor *)descriptor);
}

void BattleModelController_ConfigureResources(BattleModel *model, const void *descriptor)
{
    BattleModel_UpdateSpriteResources(model, (const ModelResourceDescriptor *)descriptor);
}

int BattleModelController_Restore(BattleModel *model, const void *descriptor, void *controller, s16 animation)
{
    model->initial_animation_id = animation;
    model->restore_resources(descriptor);
    model->animation_controller = controller;
    model->reset_controller_work();
    model->configure_animation_layer(-1, -1, 1);
    return model->set_primary_animation((u8)model->animation_id, 0, 1);
}

int BattleModelController_Configure(BattleModel *model, const void *descriptor, void *controller, s16 animation)
{
    model->initial_animation_id = animation;
    model->animation_id = animation;
    model->configure_resources(descriptor);
    model->animation_controller = controller;
    model->reset_controller_work();
    model->set_controller_animation(model->animation_id, 0);
    model->configure_animation_layer(-1, -1, 1);
    return model->set_primary_animation(0, 0, 1);
}

void BattleModelController_Reset(BattleModel *model)
{
    int index = 0;
    do {
        model->controller_work.primary_components[index] = 0;
        model->controller_work.primary_steps_q8[index] = model->effect_scale;
        ++index;
    } while (index < 8);
    model->controller_work.primary_state = 0;
    model->controller_work.primary_mode = 2;
    model->controller_work.primary_track = 0;
    index = 0;
    do {
        model->controller_work.layer_components[index] = 0;
        model->controller_work.layer_steps_q8[index] = model->effect_scale;
        model->controller_work.layer_states[index] = 0;
        model->controller_work.layer_modes[index] = 2;
        model->controller_work.layer_tracks[index] = 0;
        model->controller_work.layer_animation_ids[index] = 0;
        ++index;
    } while (index < 8);
}
}
