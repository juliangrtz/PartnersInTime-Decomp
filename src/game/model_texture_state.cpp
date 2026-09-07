#include <game/battle_scene.h>

extern "C" {
extern void func_0200b374(u32 destination, const GameGraphicsResource *resource,
                         const void *pixels, int group, int frame);

void BattleRenderModel_UpdateTexture(BattleModel *model, const u32 *banks)
{
    if (!model->is_texture_dirty()) return;
    if (!model->flag_bits.no_sort_key) {
        func_0200b374((model->render_texture.offset & 0x1FFFF) +
            banks[model->render_texture.offset >> 17], model->resource,
            model->resource_pixels, -1,
            model->frames[model->property_056 +
                model->animation_data[model->animation_id].start_frame].unknown_00);
    } else {
        func_0200b374((model->render_texture.offset & 0x1FFFF) +
            banks[model->render_texture.offset >> 17], model->resource,
            model->resource_pixels, -1, -1);
    }
    model->render_texture.state &= ~4;
}

void BattleRenderModel_NoOp(BattleModel *model) {}

u32 BattleRenderModel_GetObjectSize(BattleModel *model)
{
    return sizeof(BattleModel);
}

void BattleRenderModel_SetTextureDirty(BattleModel *model, int enabled)
{
    if (model->flag_bits.no_sort_key) return;
    if (model->flag_bits.texture_allocation_mode == 3) return;
    if (enabled) model->render_texture.state |= 4;
    else model->render_texture.state &= ~4;
}

int BattleRenderModel_IsTextureDirty(BattleModel *model)
{
    return (model->render_texture.state & 4) == 4;
}

void BattleRenderModel_SetPaletteDirty(BattleModel *model, int enabled)
{
    if (enabled) model->render_palette.state |= 0x20;
    else model->render_palette.state &= ~0x20;
}

int BattleRenderModel_IsPaletteDirty(BattleModel *model)
{
    return (model->render_palette.state & 0x20) == 0x20;
}

void GameTexture_ResetLists(void)
{
    data_0205a8b4 = 0;
    data_0205a8c0 = 0;
    data_0205a8bc = 0;
    data_0205a8b0 = 0;
}
}
