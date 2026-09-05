#include <game/overlay021_attack_state.h>

extern void func_ov002_02071750(BattleSceneObject *object, int value);
extern void func_0200940c(BattleModel *model, int scale);

/* Metrowerks emits C functions in reverse source order. */
void Overlay21Attack_SetPrimaryPhase(Overlay21AttackState *state, int phase,
                                     int duration) {
    state->flags = (state->flags & ~OVERLAY21_ATTACK_PRIMARY_PHASE_MASK) |
                   ((phase & 0xF) << 5);
    state->primary_timer = duration;
}

void Overlay21Attack_ClearPrimaryPhase(Overlay21AttackState *state) {
    state->flags &= ~OVERLAY21_ATTACK_PRIMARY_PHASE_MASK;
    state->primary_timer = 0;
}

void Overlay21Attack_StartSecondaryPhase(Overlay21AttackState *state) {
    Overlay21AttackConfig *config = state->config;

    if (config->phase_1_duration != 0) {
        state->flags =
            (state->flags & ~OVERLAY21_ATTACK_SECONDARY_PHASE_MASK) |
            OVERLAY21_ATTACK_SECONDARY_PHASE_1;
        state->secondary_timer = config->phase_1_duration;
        return;
    }

    state->flags = (state->flags & ~OVERLAY21_ATTACK_SECONDARY_PHASE_MASK) |
                   OVERLAY21_ATTACK_SECONDARY_PHASE_2;
    state->secondary_timer = config->phase_2_duration;
}

void Overlay21Attack_ClearSecondaryPhase(Overlay21AttackState *state) {
    state->flags &= ~OVERLAY21_ATTACK_SECONDARY_PHASE_MASK;
}

void Overlay21Attack_ConfigureAnimation(Overlay21AttackState *state,
                                        int animation_id, int value) {
    BattleSceneObject *object = state->object;
    BattleModel *model;

    BattleSceneObject_SetAnimation(object, animation_id, -1);
    func_ov002_02071750(object, value);
    model = BattleSceneObject_GetActiveModel(object);
    func_0200940c(model, 0x100);
    model->flags &= ~BATTLE_MODEL_FLAG_10;
    model->flags &= ~BATTLE_MODEL_FLAG_11;
}
