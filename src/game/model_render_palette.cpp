/*
 * Render model palette (ARM9 resident, 0x0200D754-0x0200D7D0).
 *
 * The palette a render model draws with: its source, its colour count, whether
 * it is buffered, and the transform and sort-key mode read alongside it.
 */

#include <game/battle_scene.h>

extern "C" {
u8 BattleRenderModel_ReadSortKeyMode(BattleModel *unused, const ModelRenderSortKey *key)
{
    return key->mode;
}

void BattleRenderModel_SetPaletteBuffered(BattleModel *model, int enabled)
{
    if (enabled) model->render_palette.state |= 0x40;
    else model->render_palette.state &= ~0x40;
}

int BattleRenderModel_GetPaletteColorCount(BattleModel *model)
{
    return model->render_palette.size >> 1;
}

const void *BattleRenderModel_GetPaletteSource(BattleModel *model)
{
    return model->render_palette.data;
}

void BattleRenderModel_SetPalette(BattleModel *model, const void *source)
{
    GameTexturePalette_MarkDirty(&model->render_palette, source);
}

void *BattleRenderModel_GetPaletteBuffer(BattleModel *model)
{
    return GameTexturePalette_GetBuffer(&model->render_palette);
}

MtxFx44 *BattleRenderModel_GetTransform(BattleModel *model)
{
    return &model->transform;
}
}
