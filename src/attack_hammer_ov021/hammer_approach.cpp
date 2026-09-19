/*
 * Hammer approach (overlay 21, 0x020C3864-0x020C395C).
 * Moves to the target's attack anchor at the formation-specific speed,
 * starts the approach animation and opens the early-input window.
 */
extern "C" {
#include <game/battle_effect.h>
#include <nitro/fx.h>
}
#include <game/overlay021_attack_state.h>

extern "C" {
extern Overlay21AttackConfig data_ov021_020c4030[];

u32 Overlay21Attack_BeginApproach(Overlay21AttackState *state) {
  BattleSceneObject *object = state->object;
  BattleSceneObject *target =
      BattleSceneObject_GetById((u16)state->actor->actor.target_actor_id);
  Overlay21AttackConfig *config =
      &data_ov021_020c4030[state->actor->formation_index - 2];
  Overlay21Attack_ConfigureAnimation(state, 1, 1);
  int dx = target->x + target->property_100 - object->x;
  int dy = target->y + target->property_101 - object->y;
  int distance = FX_Sqrt((dx * dx + dy * dy) << 12);
  BattleSceneObject_MoveBy(object, 2, dx, dy, 0,
                           distance * config->approach_duration / 0x80000);
  BattleSound_Play(61, 0, 0, 0);
  Overlay21Attack_SetPrimaryPhase(state, 1, 0);
  u32 result = (state->flags & ~31) | 2;
  state->flags = result;
  return result;
}
}
