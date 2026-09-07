#include <game/battle_scene.h>

extern "C" {
int BattleRenderModel_ConfigureController(BattleModel *model, const void *descriptor, void *controller, s16 animation)
{
    return model->configure_controller(descriptor, controller, animation);
}

void BattleRenderModel_UnlinkTexture(BattleModel *model)
{
    GameTextureAllocation_Unlink(&model->render_texture);
}

int BattleModel_SetAlpha(BattleModel *model, u8 alpha, u8 mode)
{
    model->animation_state_bits.state = alpha;
    model->animation_state_bits.mode = mode;
    /* Preserve the original adjusted receiver returned in r0. */
    return (int)((u8 *)model + 0x100);
}

void BattleRenderModels_UpdateTextures(const u32 *banks, u8 filter)
{
    BattleModel *model = gModelRenderList[0];
    while (model) {
        if (model->get_object_size() == sizeof(BattleModel)) {
            if (((filter & 1) && model->flag_bits.no_sort_key == 0) ||
                ((filter & 2) && model->flag_bits.no_sort_key == 1)) {
                model->update_texture(banks);
            }
        }
        model = model->render_next;
    }
}
}
