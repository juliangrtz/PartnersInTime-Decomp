extern "C" {
#include <nitro/fx.h>
#include <game/battle_effect.h>
}
#include <game/overlay021_attack_state.h>
extern "C" {
extern Overlay21AttackConfig data_ov021_020c4030[];
extern void func_ov002_0207179c(BattlePartyActor *, int);
BattleSceneObject *Overlay21Attack_Initialize(Overlay21AttackState *state, BattlePartyActor *actor)
{
    state->object = actor->actor.scene_object;
    state->actor = actor;
    state->config = &data_ov021_020c4030[actor->formation_index - 2];
    state->timer = 0;
    state->primary_timer = 0;
    state->secondary_timer = 0;
    state->bits.phase = 0;
    state->bits.primary_phase = 0;
    state->bits.secondary_phase = 0;
    state->bits.secondary = 0;
    BattleSceneObject *object = state->object;
    object->effect_anchor_z = -32;
    return object;
}

u32 Overlay21Attack_FinishReturn(Overlay21AttackState *state)
{
    state->object->effect_anchor_z = 0;
    func_ov002_0207179c(state->actor, 0);
    u32 result = state->flags & ~31;
    state->flags = result;
    return result;
}
}
