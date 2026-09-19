/*
 * Flexible link actor (overlay 2, 0x020BAF18-0x020BAF38).
 *
 * Binds a flexible link to its actor.
 */

#include <game/battle_object_link.h>
void BattleFlexibleLink_SetActor(BattleSceneObject *object, u16 actor) {
    BattleFlexibleLinkState *s = FLEXIBLE_LINK_STATE(object);
    s->actor = actor;
    s->flags.flag_00 = 0;
}
