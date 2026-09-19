#include "attack_internal.h"

void Overlay20Support_BeginInput(Overlay20AttackState *state)
{
    if (state->primary_object)
        state->flags.bits.state = 1;
}

void Overlay20Support_StopInput(Overlay20AttackState *state)
{
    if (state->primary_object)
        state->flags.bits.state = 0;
}

void Overlay20Support_StartReturn(Overlay20AttackState *state, Overlay20AttackState *main)
{
    BattleSceneObject *object = state->primary_object;
    BattleSceneObject *target = main->primary_object;
    if (object) {
        BattlePartyActor *actor = main->actor;
        Overlay20AttackConfig *config = &data_ov020_020c4fd0[main->flags.bits.formation_variant];
        int index, duration;
        func_ov020_020c2efc(state, 0x4301);
        index = state->flags.bits.formation_variant & 1;
        BattleSceneObject_AdjustPosition(object, target->x + data_ov020_020c4ee8[index][0] - object->x,
                                         target->y - object->y,
                                         target->z + data_ov020_020c4ee9[index][0] - object->z);
        duration = BattleSceneObject_StartVerticalArc(object, 3, config->support_velocity,
                                                      config->support_gravity, 0);
        BattleSceneObject_MoveBy(object, 2, actor->actor.unk_018 - 16 - object->x,
                                 actor->actor.unk_01a - object->y, 0, duration);
        state->flags.bits.state = 2;
    }
}

void Overlay20Support_SetMode(Overlay20AttackState *state, int mode)
{
    if (state->primary_object)
        state->flags.bits.mode = mode;
}
