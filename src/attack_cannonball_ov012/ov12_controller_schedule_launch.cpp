#include "motion_internal.h"

extern "C" {
void Overlay12Controller_Prepare(Overlay12AttackController *controller)
{
    BattleSceneObject *object = controller->primary_object;
    BattleEntity_BindResource(object->actor_id, (u16)((controller->controller_flags.bits.mode >> 1) + 57));
    BattleSceneObject_SetAnimation(object, 0, -1);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    model->flags = (model->flags & ~0xf000) | 0x1000;
    controller->flags.bits.state = 1;
}

int Overlay12Controller_IsReady(Overlay12AttackController *controller)
{
    return Overlay12Controller_IsIdle(controller);
}

void Overlay12Controller_ScheduleLaunch(Overlay12AttackController *controller, int value)
{
    controller->unknown_10 = value;
    controller->flags.bits.state = 3;
}
}
