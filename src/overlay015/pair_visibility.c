#include "actor_internal.h"

void Overlay15Attack_HideAttackObjects(Overlay15AttackObjectPairState *state)
{
    BattleSceneObject *third = BattleSceneObject_GetById(43);
    state->primary->effect_anchor_z = 0;
    state->secondary->effect_anchor_z = 0;
    third->effect_anchor_z = 0;
    BattleSceneObject_SetAnimation(state->primary, -1, -1);
    BattleSceneObject_SetAnimation(state->secondary, -1, -1);
    BattleSceneObject_SetAnimation(third, -1, -1);
}
