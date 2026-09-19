/*
 * Flexible link reset (overlay 2, 0x020BAE44-0x020BAE7C).
 *
 * Resets a flexible link to its rest state.
 */

#include <game/battle_object_link.h>
void BattleFlexibleLink_Reset(BattleSceneObject *object, int unused) {
    BattleFlexibleLinkState *s = FLEXIBLE_LINK_STATE(object);
    s->unknown_d0 = 1;
    s->flags.flag_01 = 0;
    s->flags.flag_00 = 0;
    s->actor = 0;
}
