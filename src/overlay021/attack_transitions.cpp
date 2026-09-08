extern "C" {
#include <nitro/fx.h>
#include <game/battle_effect.h>
}
#include <game/overlay021_attack_state.h>
extern "C" {
extern void func_ov002_02072004(BattleSceneObject *, int, int, int, int);
extern void func_0200940c(BattleModel *, s16);
extern void func_ov002_020718d0(BattlePartyActor *, int, int);
u32 Overlay21Attack_FinishApproach(Overlay21AttackState *state)
{
    Overlay21Attack_ConfigureAnimation(state, 4, 0);
    BattleSound_Play(50, 0, 0, 0);
    Overlay21Attack_SetPrimaryPhase(state, 2, 0);
    if (state->actor->formation_index >= 4)
        Overlay21Attack_StartSecondaryPhase(state);
    u32 result = (state->flags & ~31) | 3;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_WaitPrimaryInput(Overlay21AttackState *state)
{
    Overlay21Attack_ConfigureAnimation(state, 5, 1);
    Overlay21Attack_SetPrimaryPhase(state, 3, state->config->primary_window);
    u32 result = (state->flags & ~31) | 5;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_HitPrimary(Overlay21AttackState *state)
{
    state->bits.result = 1;
    Overlay21Attack_ClearPrimaryPhase(state);
    Overlay21Attack_ConfigureAnimation(state, 6, 0);
    state->timer = state->config->success_delay;
    u32 result = (state->flags & ~31) | 6;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_MissPrimary(Overlay21AttackState *state)
{
    state->bits.result = 2;
    Overlay21Attack_ClearPrimaryPhase(state);
    Overlay21Attack_ConfigureAnimation(state, 7, 0);
    BattleSound_Play(53, 0, 0, 0);
    state->timer = state->config->miss_delay;
    u32 result = (state->flags & ~31) | 7;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_EarlyInput(Overlay21AttackState *state)
{
    state->bits.result = 3;
    Overlay21Attack_ClearPrimaryPhase(state);
    Overlay21Attack_ConfigureAnimation(state, 2, 0);
    BattleSceneObject_StopMotionChannel(state->object, 2, 0);
    u32 result = (state->flags & ~31) | 8;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_BeginAdvance(Overlay21AttackState *state)
{
    BattleSceneObject *object = state->object;
    Overlay21AttackConfig *config = state->config;
    Overlay21Attack_ConfigureAnimation(state, 3, 1);
    func_0200940c(BattleSceneObject_GetActiveModel(object), config->advance_animation_speed);
    BattleSceneObject *target = BattleSceneObject_GetById((u16)state->actor->actor.target_actor_id);
    int dx = target->x + target->property_100 - object->x;
    int dy = target->y + target->property_101 - object->y;
    int distance = FX_Sqrt((dx * dx + dy * dy) << 12);
    BattleSceneObject_MoveBy(object, 2, dx, dy, 0, distance * config->advance_duration / 0x80000);
    func_ov002_020718d0(state->actor, 38, 39);
    state->timer = 0;
    u32 result = (state->flags & ~31) | 9;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_FinishAdvance(Overlay21AttackState *state)
{
    Overlay21Attack_ConfigureAnimation(state, 8, 0);
    BattleSceneObject *object = state->object;
    u16 formation = state->actor->formation_index;
    func_ov002_02072004(object, 0, formation == 2 || formation == 3 ? 17 : 16, 0, 0);
    state->timer = 0;
    u32 result = (state->flags & ~31) | 10;
    state->flags = result;
    return result;
}

Overlay21AttackState *Overlay21Attack_BeginReturnWait(Overlay21AttackState *state)
{
    Overlay21AttackConfig *config = state->config;
    switch (state->bits.result) {
    case 1:
        state->timer = config->success_wait;
        break;
    case 2:
        state->timer = config->miss_wait;
        break;
    case 3:
        state->timer = config->early_wait;
        break;
    }
    state->bits.phase = 11;
    return state;
}

u32 Overlay21Attack_BeginSecondary(Overlay21AttackState *state)
{
    state->bits.secondary_phase = 0;
    state->bits.secondary = 1;
    Overlay21Attack_ConfigureAnimation(state, 9, 0);
    BattleSceneObject_MoveBy(state->object, 2, state->config->secondary_distance, 0, 0,
                             state->config->secondary_duration);
    Overlay21Attack_SetPrimaryPhase(state, 2, 0);
    BattleSound_Play(46, 0, 0, 0);
    u32 result = (state->flags & ~31) | 13;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_WaitSecondaryInput(Overlay21AttackState *state)
{
    Overlay21Attack_ConfigureAnimation(state, 10, 1);
    Overlay21Attack_SetPrimaryPhase(state, 3, state->config->secondary_window);
    u32 result = (state->flags & ~31) | 14;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_HitSecondary(Overlay21AttackState *state)
{
    state->bits.result = 1;
    Overlay21Attack_ClearPrimaryPhase(state);
    Overlay21Attack_ConfigureAnimation(state, 11, 0);
    state->timer = state->config->success_delay;
    u32 result = (state->flags & ~31) | 15;
    state->flags = result;
    return result;
}

u32 Overlay21Attack_MissSecondary(Overlay21AttackState *state)
{
    state->bits.result = 2;
    Overlay21Attack_ClearPrimaryPhase(state);
    Overlay21Attack_ConfigureAnimation(state, 12, 0);
    BattleSound_Play(53, 0, 0, 0);
    state->timer = state->config->miss_delay;
    u32 result = (state->flags & ~31) | 16;
    state->flags = result;
    return result;
}
}
