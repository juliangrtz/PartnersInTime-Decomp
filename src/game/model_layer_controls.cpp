#include <game/battle_scene.h>
#include <game/palette_animation.h>

extern "C" {
void BattleModelController_SetLayerState(BattleModel *model, int layer, u8 state)
{
    if (layer == -1) {
        for (int index = 0; index < 8; ++index)
            model->controller_work.layer_states[index] = state;
    } else model->controller_work.layer_states[layer] = state;
}

void BattleModelController_SetLayerMode(BattleModel *model, int layer, u8 mode)
{
    if (layer == -1) {
        for (int index = 0; index < 8; ++index)
            model->controller_work.layer_modes[index] = mode;
    } else model->controller_work.layer_modes[layer] = mode;
}

void BattleModelController_SetLayerSpeed(BattleModel *model, int layer, s16 speed_q8, int use_model_speed)
{
    if (use_model_speed) speed_q8 = model->effect_scale;
    if (layer == -1) {
        for (int index = 0; index < 8; ++index)
            model->controller_work.layer_steps_q8[index] = speed_q8;
    } else model->controller_work.layer_steps_q8[layer] = speed_q8;
}

void BattleModelController_SetLayerAnimation(BattleModel *model, int layer, int animation, int reset)
{
    if (animation == -1 || !model->animation_controller) {
        if (layer == -1) {
            for (int index = 0; index < 8; ++index) {
                model->controller_work.layer_tracks[index] = 0;
                model->controller_work.layer_animation_ids[index] = 0;
            }
        } else {
            model->controller_work.layer_tracks[layer] = 0;
            model->controller_work.layer_animation_ids[layer] = 0;
        }
    } else if (layer == -1) {
        const s16 *track = GameAnimationTable_GetTrack(
            (const s16 *)model->animation_controller, animation);
        for (int index = 0; index < 8; ++index) {
            model->controller_work.layer_tracks[index] = track;
            model->controller_work.layer_animation_ids[index] = (u8)animation;
        }
        if (reset) {
            for (int index = 0; index < 8; ++index)
                model->controller_work.layer_components[index] = 0;
        }
    } else {
        model->controller_work.layer_tracks[layer] = GameAnimationTable_GetTrack(
            (const s16 *)model->animation_controller, animation);
        model->controller_work.layer_animation_ids[layer] = animation;
        if (reset) model->controller_work.layer_components[layer] = 0;
    }
}
}
