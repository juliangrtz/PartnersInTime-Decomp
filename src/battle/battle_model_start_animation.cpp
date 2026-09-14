#include <game/battle_scene.h>
int BattleModel_StartAnimation(BattleModel *model, int animation_id, int argument)
{
    /* A new animation clears the prior completion trigger. */
    model->flags &= ~BATTLE_MODEL_FLAG_02;
    return model->set_animation((u8)animation_id, (s16)argument);
}
