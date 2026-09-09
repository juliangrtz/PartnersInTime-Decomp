extern "C" {
#include <game/battle_callback_model.h>
#include <game/heap.h>
extern BattleModelVTable data_ov002_020be7d8;
void func_0202cbd4(void *, int, u32);
}
#define CALLBACKS ((BattleCallbackModelView *)model)

extern "C" BattleModel *BattleCallbackModel_Init(BattleModel *model)
{
    BattleModelController_Init(model);
    *(BattleModelVTable **)model = &data_ov002_020be7d8;
    /* Clear only the subclass extension after the controller constructor. */
    func_0202cbd4(&CALLBACKS->owned_buffer, 0, (u8 *)(CALLBACKS + 1) - (u8 *)&CALLBACKS->owned_buffer);
    model->screen = 1;
    return model;
}
extern "C" BattleModel *BattleCallbackModel_Destroy(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_ov002_020be7d8;
    if (CALLBACKS->owned_buffer)
        GameHeap_Delete(CALLBACKS->owned_buffer);
    BattleModelController_DestroyBase(model);
    return model;
}
extern "C" BattleModel *BattleCallbackModel_Delete(BattleModel *model)
{
    *(BattleModelVTable **)model = &data_ov002_020be7d8;
    if (CALLBACKS->owned_buffer)
        GameHeap_Delete(CALLBACKS->owned_buffer);
    BattleModelController_DestroyBase(model);
    GameHeap_Delete(model);
    return model;
}
extern "C" void BattleCallbackModel_BindPalette(BattleModel *model, const void *descriptor, void *, s16)
{
    model->flags |= 0x100;
    model->palette = *(GameSpritePalette *const *)((const u8 *)descriptor + 40);
}
extern "C" void BattleCallbackModel_ReleaseNoOp(BattleModel *model) {}
extern "C" void BattleCallbackModel_IgnoreAnimation(BattleModel *model, u8, int, int) {}
extern "C" void BattleCallbackModel_Prepare(BattleModel *model)
{
    if (CALLBACKS->prepare)
        CALLBACKS->prepare(model);
}
extern "C" void BattleCallbackModel_Draw(BattleModel *model, void *buffer, u8 *object_count, u8 *affine_count)
{
    if (CALLBACKS->draw)
        CALLBACKS->draw(model, buffer, object_count, affine_count);
}
extern "C" void BattleCallbackModel_Update(BattleModel *model)
{
    if (CALLBACKS->update)
        CALLBACKS->update(model);
}
