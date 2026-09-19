#include "attack_internal.h"

void Overlay20Attack_StartApproach(Overlay20AttackState *state)
{
    BattleSceneObject *object = state->primary_object;
    Overlay20AttackConfig *config = &data_ov020_020c4fd0[state->flags.bits.formation_variant];
    func_ov020_020c2efc(state, 0x204);
    BattleSceneObject_MoveBy(object, 2, config->approach_x, 0, 0, config->approach_duration);
    BattleSound_Play(61, 0, 0, 0);
    state->flags.bits.state = 1;
}
