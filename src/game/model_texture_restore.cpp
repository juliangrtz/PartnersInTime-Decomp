/*
 * Render model texture restore (ARM9 resident, 0x0200F804-0x0200F85C).
 *
 * The converted size of a model's texture and the release that hands its
 * resources and controller back.
 */

#include <game/battle_scene.h>

extern "C" {
int BattleRenderModel_GetTextureConversionSize(const GameGraphicsResource *resource, u32 alternate)
{
    return BattleModel_GetTextureConversionSize(3, (u8)alternate, resource);
}

int BattleRenderModel_RestoreController(BattleModel *model, const void *descriptor, void *controller, s16 animation)
{
    return model->restore_controller(descriptor, controller, animation);
}

void BattleRenderModel_ReleaseResources(BattleModel *model)
{
    GameTextureAllocation_Unlink(&model->render_texture);
    GameTexturePalette_Unlink(&model->render_palette);
}
}
