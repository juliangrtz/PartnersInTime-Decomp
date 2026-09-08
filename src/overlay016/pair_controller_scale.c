#include "pair_effects_internal.h"

void Overlay16PairController_UpdateScale(Overlay16PairController *controller)
{
    if (controller->radius) {
        int scale = (controller->radius << 8) / 32;
        BattleModel *model = BattleSceneObject_GetActiveModel(controller->primary);
        model->scale_x = scale;
        model->scale_y = scale;
        model = BattleSceneObject_GetActiveModel(controller->secondary);
        model->scale_x = scale;
        model->scale_y = scale;
    }
}
