#include <game/battle_actor.h>
#include <game/overlay012_attack_state.h>

enum {
    OVERLAY12_STATE_MASK = 0x1F,
    OVERLAY12_FORMATION_VARIANT_MASK = 0x60,
    OVERLAY12_CONTROLLER_MODE_MASK = 3,
};

extern void func_ov002_0207179c(BattlePartyActor *actor,
                                BattleSceneObject *object);
extern u16 data_ov012_020c5a2c[];

/* Metrowerks emits C functions in reverse source order. */
void Overlay12Attack_InitializeController(Overlay12AttackController *state,
                                          int mode,
                                          int formation_variant,
                                          BattleSceneObject *primary_object,
                                          BattleSceneObject *secondary_object,
                                          int argument) {
    state->primary_object = primary_object;
    state->secondary_object = secondary_object;
    state->actor = (BattlePartyActor *)BattleActor_GetById(
        data_ov012_020c5a2c[formation_variant]);
    state->argument = argument;
    state->unknown_10 = 0;
    state->counter_14 = 0;
    state->counter_16 = 0;
    state->flags.raw &= ~OVERLAY12_STATE_MASK;
    state->flags.raw =
        (state->flags.raw & ~OVERLAY12_FORMATION_VARIANT_MASK) |
        (((u8)formation_variant & 3) << 5);
    state->flags.bits.unknown_7 = 0;
    state->controller_flags.raw =
        (state->controller_flags.raw & ~OVERLAY12_CONTROLLER_MODE_MASK) |
        ((u8)mode & 3);
    primary_object->effect_anchor_z = 0;
}

int Overlay12Attack_CleanupController(Overlay12AttackController *state) {
    BattleSceneObject *primary_object = state->primary_object;
    BattleSceneObject *secondary_object = state->secondary_object;

    if (state->flags.bits.formation_variant <= 1) {
        func_ov002_0207179c(state->actor, primary_object);
    } else {
        BattleSceneObject_SetAnimation(primary_object, -1, -1);
    }
    BattleSceneObject_SetAnimation(secondary_object, -1, 1);
    primary_object->effect_anchor_z = 0;
    secondary_object->effect_anchor_z = 0;
    return 0;
}
