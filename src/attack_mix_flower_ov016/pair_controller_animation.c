/*
 * Mix Flower pair animation (overlay 16, 0x020C36E0-0x020C3854).
 *
 * Starts the paired controller's animation and scales it each frame.
 */

#include "pair_effects_internal.h"

void Overlay16PairController_StartAnimation(Overlay16PairController *controller)
{
    BattleSceneObject *primary = controller->primary;
    BattleSceneObject *secondary = controller->secondary;

    BattleEntity_BindResource(primary->actor_id, 52);
    BattleEntity_BindResource(secondary->actor_id, 52);
    BattleSceneObject_SetAnimation(primary, 1, -1);
    BattleSceneObject_SetAnimation(secondary, 1, 1);
    primary->flags.bits.unk_20_23 = 1;
    secondary->flags.bits.unk_20_23 = 1;
    func_ov002_02071938(secondary, controller->primary, 76);
    BattleModelAnimation_StartAttached(&controller->animation,
        849, BattleSceneObject_GetActiveModel(secondary), 0, 0, 0, 256);
    controller->position_z_q12 = controller->primary->z << 12;
    controller->unknown24 = 0;
    controller->radius = 20;
    controller->unknown16 = Overlay16PairController_GetMotionStep(controller);
    controller->phase = 1;
    BattleSound_Play(0, 0, 0, 0);
}

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
