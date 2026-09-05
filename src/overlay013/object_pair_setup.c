#include <game/battle_object.h>
#include <game/overlay013_attack.h>

enum {
    OVERLAY13_PRIMARY_ANIMATION_COMPONENT = 0,
    OVERLAY13_SECONDARY_ANIMATION_COMPONENT = 1,
    OVERLAY13_PAIR_ALIGNMENT_Z = 74,
    OVERLAY13_RESOURCE_BASE = 57,
    OVERLAY13_ANIMATION_BASE = 4,
};

extern int func_ov002_02071938(BattleSceneObject *object,
                               BattleSceneObject *reference,
                               int offset_z);
extern int data_ov013_020c5b00;
extern int data_ov013_020c5b08;
extern int data_ov013_020c5b0c;
extern int data_ov013_020c5b14;

/* Metrowerks emits C functions in reverse source order. */
void Overlay13Attack_PrepareObjectPair(Overlay13AttackPairState *state) {
    BattleSceneObject *primary = state->primary_object;
    BattleSceneObject *secondary = state->secondary_object;
    BattleModel *model;

    BattleEntity_BindResource(
        primary->actor_id,
        (state->flags.bits.variant & 1) ?
            OVERLAY13_RESOURCE_BASE + 1 : OVERLAY13_RESOURCE_BASE);
    BattleEntity_BindResource(
        secondary->actor_id,
        (state->flags.bits.variant & 1) ?
            OVERLAY13_RESOURCE_BASE + 1 : OVERLAY13_RESOURCE_BASE);
    BattleSceneObject_SetAnimationFromComponent(
        primary,
        state->flags.bits.variant <= 1 ?
            OVERLAY13_ANIMATION_BASE : OVERLAY13_ANIMATION_BASE + 1,
        OVERLAY13_PRIMARY_ANIMATION_COMPONENT, state->animation_component);
    BattleSceneObject_SetAnimation(
        secondary,
        state->flags.bits.variant <= 1 ?
            OVERLAY13_ANIMATION_BASE : OVERLAY13_ANIMATION_BASE + 1,
        OVERLAY13_SECONDARY_ANIMATION_COMPONENT);
    model = BattleSceneObject_GetActiveModel(primary);
    model->flags =
        (model->flags & ~BATTLE_MODEL_ANIMATION_MODE_MASK) |
        BATTLE_MODEL_ANIMATION_MODE_STATUS;
    model = BattleSceneObject_GetActiveModel(secondary);
    model->flags =
        (model->flags & ~BATTLE_MODEL_ANIMATION_MODE_MASK) |
        BATTLE_MODEL_ANIMATION_MODE_STATUS;
}

void Overlay13Attack_AlignObjectPair(Overlay13AttackPairState *state) {
    func_ov002_02071938(state->secondary_object, state->primary_object,
                        OVERLAY13_PAIR_ALIGNMENT_Z);
}

void Overlay13Attack_StartRetreat(BattleSceneObject *object) {
    int duration;

    BattleSceneObject_SetAnimation(object, 3, -1);
    BattleSceneObject_GetActiveModel(object)->flags &=
        ~BATTLE_MODEL_ANIMATION_MODE_MASK;
    BattleSceneObject_GetActiveModel(object)->flags &= ~BATTLE_MODEL_FLAG_09;
    duration = BattleSceneObject_StartVerticalArc(
        object, 3, 0x400, 20, -64 - (192 - object->y));
    BattleSceneObject_MoveBy(object, 2, -20, 0, 0, duration);
}

void Overlay13Attack_AdvanceMotionTuning(void) {
    data_ov013_020c5b0c += 0x8000;
    data_ov013_020c5b00 += 0x4000;
    data_ov013_020c5b14 += 0x1200;
    data_ov013_020c5b08 += 0x12;
}
