#include <game/battle_object_link.h>
void BattleFlexibleLink_Reset(BattleSceneObject *object, int unused) {
    BattleFlexibleLinkState *s = FLEXIBLE_LINK_STATE(object);
    s->unknown_d0 = 1;
    s->flags.flag_01 = 0;
    s->flags.flag_00 = 0;
    s->actor = 0;
}
