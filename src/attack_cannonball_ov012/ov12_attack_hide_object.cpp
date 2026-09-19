/*
 * Cannonballer object visibility (overlay 12, 0x020C3480-0x020C34E0).
 *
 * Initializes the attack object and hides it again when its phase is over.
 */

#include "motion_internal.h"

extern "C" {
void Overlay12Attack_InitializeObject(Overlay12AttackState *state)
{
    state->object = BattleSceneObject_GetById(40);
    state->timer = 0;
    state->counter = 0;
    state->animation_step = 0;
    state->flags.bits.state = 0;
}

int Overlay12Attack_HideObject(Overlay12AttackState *state)
{
    BattleSceneObject *object = state->object;
    BattleSceneObject_SetAnimation(object, -1, -1);
    object->effect_anchor_z = 0;
    return 0;
}
}
