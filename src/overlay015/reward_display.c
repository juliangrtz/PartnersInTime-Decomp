#include "actor_internal.h"

void Overlay15Attack_ShowRewardItem(const Overlay15AttackRewardItemPrefix *item,
    int message_width)
{
    BattleSceneObject *object = BattleSceneObject_GetById(47);
    BattleModel *model;

    BattleSceneObject_SetAnimation(BattleSceneObject_GetById(47),
        item->animation_id, -1);
    object->flags.raw |= 0x20000;
    object->flags.raw |= 0x10000;
    object->effect_anchor_z = -3904;
    BattleSceneObject_AdjustPosition(object, -object->x, -object->y, -object->z);
    model = BattleSceneObject_GetActiveModel(object);
    model->animation_offset_x = 132 - message_width / 2;
    model->animation_offset_y = 26;
    BattleMotion_StartAppear(object, 0, 8);
}
