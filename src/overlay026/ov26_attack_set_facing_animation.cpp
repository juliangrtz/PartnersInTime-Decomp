#include "item_internal.h"

extern "C" {
void Overlay26Attack_SetFacingAnimation(BattlePartyActor *actor)
{
    BattleSceneObject_SetAnimation(actor->actor.scene_object, (actor->formation_index & 1) ? 5 : 9, -1);
}
}
