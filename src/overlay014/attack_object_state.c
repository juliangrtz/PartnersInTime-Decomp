#include <game/battle_effect.h>
#include <game/overlay014_attack.h>

enum Overlay14AttackObjectPhase {
    OVERLAY14_OBJECT_PHASE_IDLE = 0,
    OVERLAY14_OBJECT_PHASE_APPROACH = 1,
    OVERLAY14_OBJECT_PHASE_TARGET_MOVE = 2,
    OVERLAY14_OBJECT_PHASE_HIDE = 3,
};

extern int func_ov002_0206f0bc(BattleSceneObject *object, int argument_1,
                               int argument_2, int argument_3,
                               int argument_4, int argument_5);
extern int func_ov002_02071e40(int effect_id, BattleSceneObject *object,
                               int offset_x, int offset_y,
                               int offset_z, int scale);
extern int func_ov002_02071f08(int effect_id, BattleSceneObject *object,
                               int offset_x, int offset_y, int offset_z,
                               int argument_5, int scale);

typedef struct Overlay14AttackTargetParameters {
    u8 unknown_00[2];
    s8 offset_x;
    s8 target_z;
    u8 unknown_04;
} Overlay14AttackTargetParameters;

typedef struct Overlay14AttackTargetOffset {
    u8 unknown_00[5];
    Overlay14AttackTargetParameters parameters;
} Overlay14AttackTargetOffset;

extern Overlay14AttackTargetOffset data_ov014_020c6204[];

/* Metrowerks emits C functions in reverse source order. */
void Overlay14Attack_Initialize(Overlay14AttackObjectState *state,
                                BattleSceneObject *object,
                                s16 effect_anchor_z) {
    state->object = object;
    state->effect = 0;
    state->timer = 0;
    state->state = OVERLAY14_OBJECT_PHASE_IDLE;
    object->effect_anchor_z = effect_anchor_z;
}

void Overlay14Attack_Stop(Overlay14AttackObjectState *state) {
    state->state = OVERLAY14_OBJECT_PHASE_IDLE;
    BattleSceneObject_SetAnimation(state->object, -1, -1);
    state->object->effect_anchor_z = 0;
}

int Overlay14Attack_Update(Overlay14AttackObjectState *state) {
    int result = state->state;
    BattleSceneObject *object = state->object;

    switch (result) {
    case OVERLAY14_OBJECT_PHASE_IDLE:
        break;
    case OVERLAY14_OBJECT_PHASE_APPROACH:
        result = BattleSceneObject_IsAnimationChannelActive(object, 2);
        if (!result) {
            result = 0;
            state->state = OVERLAY14_OBJECT_PHASE_IDLE;
        }
        break;
    case OVERLAY14_OBJECT_PHASE_TARGET_MOVE:
        result = BattleSceneObject_IsAnimationChannelActive(object, 2);
        if (!result) {
            result = Overlay14Attack_BeginHide(state);
        }
        break;
    case OVERLAY14_OBJECT_PHASE_HIDE:
        result = BattleSceneObject_IsAnimationChannelActive(object, 0);
        if (!result) {
            BattleSceneObject_SetAnimation(object, -1, -1);
            state->state = OVERLAY14_OBJECT_PHASE_IDLE;
            result = 0;
        }
        break;
    default:
        return result;
    }
    return result;
}

int Overlay14Attack_IsIdle(Overlay14AttackObjectState *state) {
    return state->state == OVERLAY14_OBJECT_PHASE_IDLE;
}

int Overlay14Attack_StartApproach(Overlay14AttackObjectState *state) {
    BattleSceneObject *object = state->object;

    BattleSceneObject_StartAcceleratedMotionForDuration(
        object, 2, 0, 0, 1, -object->z, 32, 40, 1);
    BattleSceneObject_MoveBy(
        object, 1, 56 - object->x, 132 - object->y, 0, 32);
    state->state = OVERLAY14_OBJECT_PHASE_APPROACH;
    return OVERLAY14_OBJECT_PHASE_APPROACH;
}

int Overlay14Attack_FinishWithEffects(Overlay14AttackObjectState *state) {
    BattleSceneObject *object = state->object;

    func_ov002_02071e40(538, object, 0, -12, 0, 0x100);
    func_ov002_02071f08(844, object, 0, 0, -12, 0, 0x100);
    BattleSceneObject_SetAnimation(object, -1, -1);
    BattleSound_Play(154, 0, 0, 0);
    state->state = OVERLAY14_OBJECT_PHASE_IDLE;
    return OVERLAY14_OBJECT_PHASE_IDLE;
}

int Overlay14Attack_BeginHide(Overlay14AttackObjectState *state) {
    func_ov002_0206f0bc(state->object, 0, -1, 0, 4, 0);
    state->state = OVERLAY14_OBJECT_PHASE_HIDE;
    return OVERLAY14_OBJECT_PHASE_HIDE;
}

int Overlay14Attack_BeginTargetMove(
    Overlay14AttackObjectState *state,
    Overlay14AttackTargetReference *target_reference, int duration) {
    Overlay14AttackTarget *target = target_reference->target;
    BattleSceneObject *target_object = target->object;
    Overlay14AttackTargetParameters *parameters =
        &data_ov014_020c6204[target->offset_table_index].parameters;
    BattleSceneObject *object = state->object;
    BattleModel *model;

    BattleSceneObject_MoveTo(
        object, 2, target_object->x + parameters->offset_x,
        target_object->y + 2, parameters->target_z, duration);
    BattleSound_Play(301, 0, 0, 0);
    BattleSceneObject_SetAnimation(object, 10, -1);
    model = BattleSceneObject_GetActiveModel(object);
    BattleModel_SetAlpha(model, 31, 0);
    model = BattleSceneObject_GetActiveModel(object);
    model->scale_x = 0x100;
    model->scale_y = 0x100;
    state->state = OVERLAY14_OBJECT_PHASE_TARGET_MOVE;
    return OVERLAY14_OBJECT_PHASE_TARGET_MOVE;
}

int Overlay14Attack_LaunchAtTarget(
    Overlay14AttackObjectState *state,
    Overlay14AttackTargetReference *target_reference) {
    BattleSceneObject *object = state->object;
    BattleSceneObject *target_object = target_reference->target->object;

    BattleSceneObject_AdjustPosition(
        object, target_object->x - object->x,
        target_object->y - object->y, 64 - object->z);
    return Overlay14Attack_BeginTargetMove(state, target_reference, 8);
}
