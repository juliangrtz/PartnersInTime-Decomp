/* Selects the landing effect variant for an adult or baby formation. */
#include "attack_internal.h"
extern "C" {
void func_ov002_02072004(BattleSceneObject *, const s8 *, int, int, int);
void Overlay20Attack_SpawnLandingEffect(Overlay20AttackState *state, int normal, int babies)
{
    int formation = state->flags.bits.formation_variant;
    int is_baby = 1;
    if (formation != 2 && formation != 3)
        is_baby = 0;
    func_ov002_02072004(state->primary_object, 0, is_baby ? babies : normal, 0, 0);
}
}
