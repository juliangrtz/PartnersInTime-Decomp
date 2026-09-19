#include "actor_internal.h"

enum {
    OVERLAY15_HIT_SOURCE_ID = 40,
    OVERLAY15_HIT_KIND = 63,
};

/* Metrowerks emits C functions in reverse source order. */
int Overlay15Attack_IsLinkedPhase(Overlay15AttackPhaseState *state) {
    int result = 1;
    int phase = state->flags.bits.phase;

    if (phase == 5) {
        return result;
    }
    if (phase != 12) {
        result = 0;
    }
    return result;
}

void Overlay15Attack_ConfigureHitDescriptor(void) {
    Overlay15AttackContext *context = data_ov002_020c0710;
    int actor_id = context->actors[context->pair.flags.bits.alternate_actor]
                       .primary->object->actor_id;

    BattleHitDescriptor_Configure(
        OVERLAY15_HIT_SOURCE_ID, actor_id,
        Overlay15Attack_EmptyHitCallback, actor_id, OVERLAY15_HIT_KIND);
}

void Overlay15Attack_EmptyHitCallback(BattleHitRecord *record) {
}

int Overlay15Attack_ClampContextIndex(int offset) {
    int result = data_ov002_020c0710->index + offset;

    if (result < 0) {
        return 0;
    }
    if (result >= 30) {
        result = 29;
    }
    return result;
}

void Overlay15Attack_ConfigureModelFlags(
    Overlay15AttackModelController *state) {
    BattleModel *model;

    if (state->primary->formation_index & 1) {
        return;
    }

    model = BattleSceneObject_GetActiveModel(state->primary->object);
    model->flags |= BATTLE_MODEL_FLAG_10;
    model->flags &= ~BATTLE_MODEL_FLAG_11;

    if (state->hit_motion.object == 0) {
        return;
    }
    if (BattleSceneObject_GetActiveModel(state->hit_motion.object) == 0) {
        return;
    }

    model = BattleSceneObject_GetActiveModel(state->hit_motion.object);
    model->flags |= BATTLE_MODEL_FLAG_10;
    model->flags &= ~BATTLE_MODEL_FLAG_11;
}
