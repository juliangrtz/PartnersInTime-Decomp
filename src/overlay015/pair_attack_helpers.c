#include <game/overlay015_attack.h>

enum {
    OVERLAY15_HIT_SOURCE_ID = 40,
    OVERLAY15_HIT_KIND = 63,
};

typedef struct Overlay15AttackActorOwner {
    BattleSceneObject *object;
} Overlay15AttackActorOwner;

typedef struct Overlay15AttackActorSelection {
    Overlay15AttackActorOwner *owner;
    u8 unknown_04[0x30];
} Overlay15AttackActorSelection;

typedef union Overlay15AttackContextFlags {
    u8 raw;
    struct {
        u8 unknown_0_3 : 4;
        u8 alternate_actor : 1;
        u8 unknown_5_7 : 3;
    } bits;
} Overlay15AttackContextFlags;

typedef struct Overlay15AttackContext {
    u8 unknown_000[0xCC];
    int index;
    u8 unknown_0d0[0x60];
    Overlay15AttackActorSelection actors[2];
    u8 unknown_198[0x16];
    Overlay15AttackContextFlags flags;
} Overlay15AttackContext;

extern Overlay15AttackContext *data_ov000_020c0710;

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
    Overlay15AttackContext *context = data_ov000_020c0710;
    int actor_id = context->actors[context->flags.bits.alternate_actor]
                       .owner->object->actor_id;

    BattleHitDescriptor_Configure(
        OVERLAY15_HIT_SOURCE_ID, actor_id,
        Overlay15Attack_EmptyHitCallback, actor_id, OVERLAY15_HIT_KIND);
}

void Overlay15Attack_EmptyHitCallback(BattleHitRecord *record) {
}

int Overlay15Attack_ClampContextIndex(int offset) {
    int result = data_ov000_020c0710->index + offset;

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

    if (state->primary->flags & 1) {
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
