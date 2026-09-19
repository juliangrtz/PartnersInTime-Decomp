/*
 * Battle item facing (overlay 26, 0x020C4988-0x020C49AC).
 *
 * Sets the animation for the direction the actor is facing.
 */

#include "item_internal.h"

extern "C" {
void Overlay26Attack_SetFacingAnimation(BattlePartyActor *actor)
{
    BattleSceneObject_SetAnimation(actor->actor.scene_object, (actor->formation_index & 1) ? 5 : 9, -1);
}
}
