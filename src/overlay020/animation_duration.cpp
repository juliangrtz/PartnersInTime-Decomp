#include "attack_internal.h"

extern "C" {
void Overlay20Attack_FitAnimationDuration(BattleSceneObject *object, int duration)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    int animation = model->get_animation_id();
    int frames = BattleAnimation_GetDurationThroughFrame(object->resource->data, animation, -1, 256);
    func_0200940c(model, (s16)((frames << 8) / duration));
}
}
