#include <game/battle_object_link.h>
void BattleObjectLink_FollowSource(BattleSceneObject *object, int unused) {
    BattleObjectLinkState *s = OBJECT_LINK_STATE(object);
    s->flags.follow_source = 1;
    s->flags.follow_target = 0;
}
