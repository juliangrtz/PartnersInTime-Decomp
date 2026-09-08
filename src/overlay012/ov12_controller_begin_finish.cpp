#include "motion_internal.h"

extern "C" {
void Overlay12Controller_ContinueArc(Overlay12AttackController *controller)
{
    BattleSceneObject *object = controller->primary_object;
    BattleMotionChannel *channel = BattleSceneObject_GetMotionChannel(object, 3);
    int elapsed = channel->elapsed_q8;
    int parameter = Overlay12Attack_GetMotionParameterPair((Overlay12AttackState *)controller);
    BattleSceneObject_StartVerticalMotionTo(
        object, 3, data_ov012_020c5a04 + 2 * (elapsed / 256 * parameter / 16), 2 * (parameter / 16), 0, 1);
    channel->callback = Overlay12Motion_UpdateDampedArc;
    channel->has_deferred_delta = 0;
    controller->flags.bits.state = 18;
}

void Overlay12Controller_UpdateArcAnimation(Overlay12AttackController *controller)
{
    BattleSceneObject *primary = controller->primary_object;
    BattleSceneObject *secondary = controller->secondary_object;
    BattleMotionChannel *channel = BattleSceneObject_GetMotionChannel(primary, 3);
    int duration = channel->duration;
    int elapsed = channel->elapsed_q8 / 256;
    int third = duration / 3;
    int animation;
    if (elapsed > 2 * third)
        animation = 9;
    else if (elapsed > third)
        animation = 8;
    else
        animation = 7;
    if (!animation)
        return;
    BattleModel *model = BattleSceneObject_GetActiveModel(primary);
    if (animation != model->get_animation_id()) {
        BattleSceneObject_SetAnimation(primary, animation, 0);
        BattleSceneObject_SetAnimation(secondary, animation, 1);
        BattleSceneObject_GetActiveModel(primary)->flags &= ~0xf000;
        BattleSceneObject_GetActiveModel(secondary)->flags &= ~0xf000;
        if (animation == 7)
            BattleSound_Play(44, 0, 0, 0);
    }
}

int Overlay12Controller_IsIdle(Overlay12AttackController *controller)
{
    return controller->flags.bits.state == 0;
}

void Overlay12Controller_BeginFinish(Overlay12AttackController *controller)
{
    BattleSceneObject *object = controller->primary_object;
    if (controller->flags.bits.formation_variant <= 1) {
        BattleEntity_BindResource(object->actor_id, (u16)(!controller->controller_flags.bits.mode ? 57 : 58));
        BattleSceneObject_SetAnimation(object, 1, -1);
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        model->flags = (model->flags & ~0xf000) | 0x1000;
        controller->flags.bits.state = 2;
    } else {
        BattleSceneObject_SetAnimation(object, -1, -1);
        controller->flags.bits.state = 0;
    }
}
}
