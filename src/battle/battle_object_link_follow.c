/*
 * Object link following (overlay 2, 0x020BA0B0-0x020BA0D8).
 *
 * Makes a link follow its source object.
 */

#include <game/battle_object_link.h>
void BattleObjectLink_FollowSource(BattleSceneObject *object, int unused) {
    BattleObjectLinkState *s = OBJECT_LINK_STATE(object);
    s->flags.follow_source = 1;
    s->flags.follow_target = 0;
}
