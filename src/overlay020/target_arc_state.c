#include <game/battle_effect.h>
#include <game/battle_scene.h>
#include <game/overlay020_attack_state.h>

enum {
    OVERLAY20_STATE_MASK = 0x001F,
    OVERLAY20_FORMATION_VARIANT_MASK = 0x01E0,
    OVERLAY20_MODE_MASK = 0x0600,
    OVERLAY20_PRIMARY_OBJECT_ID = 40,
    OVERLAY20_FINISH_SOUND_ID = 223,
    OVERLAY20_FINISH_STATE = 30,
};

extern void func_ov020_020c2efc(Overlay20AttackState *state,
                                int animation_command);
extern void func_ov020_020c2ae0(BattleSceneObject *object, int value);
extern void func_ov002_0207179c(BattlePartyActor *actor,
                                BattleSceneObject *object);
extern void func_ov010_020c1304(BattleSceneObject *object,
                                BattleSceneObject *target,
                                int *parameter_x, int *parameter_y);
extern Overlay20AttackConfig data_ov020_020c4fd0[];

/* Metrowerks emits C functions in reverse source order. */
void Overlay20Attack_StartTargetArc(Overlay20AttackState *state) {
    Overlay20AttackConfig *config =
        &data_ov020_020c4fd0[state->flags.bits.formation_variant];
    BattleSceneObject *object = state->primary_object;
    BattleSceneObject *target =
        BattleSceneObject_GetById((u16)state->actor->actor.target_actor_id);
    int parameter_x;
    int parameter_y;
    int duration;

    func_ov020_020c2efc(state, 0xC008);
    parameter_x = config->arc_parameter_x;
    parameter_y = config->arc_parameter_y;
    func_ov010_020c1304(object, target, &parameter_x, &parameter_y);
    duration = BattleSceneObject_StartVerticalArc(
        object, 3, parameter_x, parameter_y, target->z + target->property_0fa);
    func_ov020_020c2ae0(object, duration);
    BattleSound_Play(39, 0, 0, 0);
    state->motion_duration = duration;
    state->flags.raw &= ~OVERLAY20_MODE_MASK;
    state->flags.raw =
        (state->flags.raw & ~OVERLAY20_STATE_MASK) | 19;
}

void Overlay20Attack_SetMode(Overlay20AttackState *state, int mode) {
    state->flags.raw = (state->flags.raw & ~OVERLAY20_MODE_MASK) |
                       (((u16)mode & 3) << 9);
    if (mode == 3 && state->flags.bits.state == 5) {
        func_ov020_020c2efc(state, 0x4007);
    }
}

void Overlay20Attack_BeginFinish(Overlay20AttackState *state,
                                 Overlay20AttackState *other) {
    Overlay20Attack_StopObjects(other);
    func_ov020_020c2efc(state, 0x4101);
    BattleSound_Play(OVERLAY20_FINISH_SOUND_ID, 0, 0, 0);
    state->flags.raw =
        (state->flags.raw & ~OVERLAY20_STATE_MASK) | OVERLAY20_FINISH_STATE;
}

void Overlay20Attack_ResetActorPosition(Overlay20AttackState *state) {
    BattlePartyActor *actor = state->actor;
    BattleSceneObject *object = actor->actor.scene_object;

    BattleSceneObject_AdjustPosition(
        object, actor->actor.unk_018 - object->x,
        actor->actor.unk_01a - object->y, -object->z);
    func_ov002_0207179c(actor, object);
}

void Overlay20Attack_Initialize(Overlay20AttackState *state,
                                BattlePartyActor *actor) {
    state->primary_object =
        BattleSceneObject_GetById(OVERLAY20_PRIMARY_OBJECT_ID);
    state->actor = actor;
    state->animation_argument = 0x980;
    state->unknown_10 = 0;
    state->unknown_14 = 0;
    state->motion_duration = 0;
    state->flags.raw &= ~OVERLAY20_STATE_MASK;
    state->flags.raw =
        (state->flags.raw & ~OVERLAY20_FORMATION_VARIANT_MASK) |
        ((actor->formation_index & 0xF) << 5);
}

void Overlay20Attack_StopObjects(Overlay20AttackState *state) {
    if (state->primary_object != 0) {
        func_ov020_020c2efc(state, -1);
    }
}
