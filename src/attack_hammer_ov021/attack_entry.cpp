extern "C" {

/*
 * Hammer entry (overlay 21, 0x020C395C-0x020C39B0).
 *
 * Begins the hammer attack's entry.
 */

#include <nitro/fx.h>
#include <game/battle_effect.h>
}
#include <game/overlay021_attack_state.h>
extern "C" {
u32 Overlay21Attack_BeginEntry(Overlay21AttackState *state)
{
    BattleEntity_BindResource(state->object->actor_id, 52);
    Overlay21Attack_ConfigureAnimation(state, 0, 0);
    BattleSound_Play(50, 0, 0, 0);
    u32 result = (state->flags & ~31) | 1;
    state->flags = result;
    return result;
}
}
