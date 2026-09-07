#include <game/battle_scene.h>

extern "C" {
int func_0200b2b0(BattleModel *, int, int);

u32 BattleModelController_GetObjectSize(BattleModel *model)
{
    return 0x130;
}

void BattleModelController_SetPrimaryAnimation(BattleModel *model, u8 animation, int frame, int update_controller)
{
    func_0200b2b0(model, (u8)(animation + (u16)model->initial_animation_id), frame);
    if (update_controller) model->set_controller_animation(model->animation_id, 0);
}

void BattleModelController_SetPrimaryState(BattleModel *model, u8 state)
{
    model->controller_work.primary_state = state;
}

void BattleModelController_SetPrimaryMode(BattleModel *model, u8 mode)
{
    model->controller_work.primary_mode = mode;
}

void BattleModelController_SetPrimarySpeed(BattleModel *model, int component, s16 speed_q8, int use_model_speed)
{
    if (use_model_speed) speed_q8 = model->effect_scale;
    if (component == -1) {
        for (int index = 0; index < 8; ++index)
            model->controller_work.primary_steps_q8[index] = speed_q8;
    } else model->controller_work.primary_steps_q8[component] = speed_q8;
}
}
