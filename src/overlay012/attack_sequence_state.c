#include <game/battle_actor.h>
#include <game/overlay012_attack_state.h>

enum {
    OVERLAY12_STATE_MASK = 0x1F,
    OVERLAY12_PARTY_ACTOR_FIRST = BATTLE_ACTOR_MARIO,
    OVERLAY12_ACTION_COUNT = 4,
    OVERLAY12_ANIMATION_STEP_DIVISOR = 30,
};

extern int func_ov002_020716bc(u8 *resource_data, int animation_id,
                               int animation_frame, int scale);
extern void func_0200940c(BattleModel *model, int value);
extern u32 Random_NextModulo(u32 modulus);

/* Metrowerks emits C functions in reverse source order. */
void Overlay12Attack_ShuffleActionOrder(Overlay12AttackState *state) {
    u8 candidates[8];
    u8 *output;
    u8 *candidate;
    int remaining;
    int value;
    u8 *base;

    output = state->action_order;
    candidate = candidates;
    value = 0;
    do {
        *candidate++ = value;
        value++;
    } while (value < OVERLAY12_ACTION_COUNT);

    /*
     * The original loop runs once with remaining == 0. Its fifth output byte
     * aliases flags, whose state bits are replaced by the caller immediately.
     */
    /* Preserve the original independent initialization schedule. */
    asm {
        mov remaining, #OVERLAY12_ACTION_COUNT
        add base, sp, #0
    }
    do {
        int index = Random_NextModulo(remaining);
        int selected = base[index];
        u8 *destination = &base[index];

        *output = selected;
        if (index < OVERLAY12_ACTION_COUNT - 1) {
            do {
                index++;
                selected = *(u8 *)((u32)base + index);
                *destination++ = selected;
            } while (index < OVERLAY12_ACTION_COUNT - 1);
        }
        remaining--;
        output++;
    } while (remaining >= 0);
}

void Overlay12Attack_StartAnimationFour(Overlay12AttackState *state) {
    BattleSceneObject *object = state->object;
    BattleModel *model;

    BattleSceneObject_SetAnimation(object, 4, -1);
    model = BattleSceneObject_GetActiveModel(object);
    model->flags |= BATTLE_MODEL_FLAG_09;
    state->animation_step =
        (func_ov002_020716bc(object->resource->data, 4, -1, 0x100) << 8) /
        OVERLAY12_ANIMATION_STEP_DIVISOR;
    model = BattleSceneObject_GetActiveModel(object);
    func_0200940c(model, (s16)state->animation_step);
    Overlay12Attack_ShuffleActionOrder(state);
    state->timer = 0;
    state->counter = 0;
    state->flags.raw =
        (state->flags.raw & ~OVERLAY12_STATE_MASK) | 3;
}

void Overlay12Attack_StartAnimationThree(Overlay12AttackState *state) {
    BattleSceneObject *object = state->object;
    BattleModel *model;

    BattleSceneObject_SetAnimation(object, 3, -1);
    model = BattleSceneObject_GetActiveModel(object);
    model->flags |= BATTLE_MODEL_FLAG_09;
    state->flags.raw =
        (state->flags.raw & ~OVERLAY12_STATE_MASK) | 1;
}

int Overlay12Attack_IsIdle(Overlay12AttackState *state) {
    return state->flags.bits.state == 0;
}

void Overlay12Attack_GetHomePosition(Overlay12AttackState *state,
                                     s16 *position_x, s16 *position_y) {
    BattleActor *actor = BattleActor_GetById(
        (u16)((state->flags.bits.formation_variant & 1) +
              OVERLAY12_PARTY_ACTOR_FIRST));
    s16 x = actor->unk_018;
    s16 y = actor->unk_01a;

    if (state->flags.bits.formation_variant >= 2) {
        x -= 16;
    }
    *position_x = x;
    *position_y = y;
}

int Overlay12Attack_GetMotionParameterPair(Overlay12AttackState *state) {
    BattleMotionChannel *channel =
        BattleSceneObject_GetMotionChannel(state->object, 3);

    return *(int *)&channel->parameters[0];
}
