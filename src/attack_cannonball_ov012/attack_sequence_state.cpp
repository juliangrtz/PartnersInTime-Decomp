extern "C" {

/*
 * Cannonballer sequence (overlay 12, 0x020C2C10-0x020C31E8).
 *
 * The order the party acts in (shuffled per use), the home positions they
 * return to, the animation each phase plays, the damped arc the cannonball
 * follows, and the hit display.
 */

#include <game/battle_actor.h>
#include <game/overlay012_attack_state.h>

enum {
    OVERLAY12_STATE_MASK = 0x1F,
    OVERLAY12_PARTY_ACTOR_FIRST = BATTLE_ACTOR_MARIO,
    OVERLAY12_ACTION_COUNT = 4,
    OVERLAY12_ANIMATION_STEP_DIVISOR = 30
};
extern int BattleAnimation_GetDurationThroughFrame(u8 *resource_data, int animation_id,
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
        (BattleAnimation_GetDurationThroughFrame(object->resource->data, 4, -1, 0x100) << 8) /
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

}

#include "motion_internal.h"

extern "C" {
void Overlay12Motion_UpdateDampedArc(BattleSceneObject *object, BattleMotionChannel *channel)
{
    int frame = channel->elapsed_q8 / 256;
    Overlay12ArcParameters *arc = (Overlay12ArcParameters *)channel->parameters;
    if (frame == channel->duration) {
        int end = arc->end;
        int start = arc->start;
        int delta = end - start;
        if (delta > 270)
            object->z = (int)((double)(start + 270) + 179.2 * (double)(delta - 270) / 256.0);
        else
            object->z = end;
    } else {
        int height = frame * (arc->velocity + arc->acceleration * frame) / 4096;
        if (height > 270)
            height = (int)(270.0 + 179.2 * (double)(height - 270) / 256.0);
        object->z = arc->start + height;
    }
}

void Overlay12Attack_ShowHit(int actor_id, Overlay12AttackController *controller, int unused, int bonus)
{
    Overlay12AttackWork *work = data_ov002_020c0710;
    BattleActor_GetById(actor_id);
    BattleSceneObject *object = BattleSceneObject_GetById(actor_id);
    int offset_y = object->property_0ff;
    int offset_z = object->property_0fa;
    int popup_x = object->property_0fe;
    int popup_y = offset_y - offset_z;
    BattlePosition position;
    BattlePosition_StoreViewRelative(
        &position, (s16)(object->x + popup_x),
        (s16)((s16)(object->y + offset_y) - (s16)(object->z + offset_z)),
        (s16)(object->effect_anchor_z + 16 * (256 - (s16)(object->y + offset_y))),
        object->flags.bits.use_raw_position, object->flags.bits.use_alternate_model);
    BattleActor_GetById(data_ov012_020c5a34[controller->flags.bits.formation_variant]);
    if (((Overlay12ControllerOptions *)&controller->controller_flags)->result == 1) {
        if (bonus) {
            BattleEffect_SetVariant(6);
            BattleSpriteEffect_Spawn(286, position.x, position.y, position.z, 256);
            BattleModelEffect_Spawn(406, 0, position.x, position.y, position.z, 256);
            Overlay10Enemy_PlayHitReaction(actor_id, 407);
            BattleSound_Play(49, 0, 0, 0);
        } else {
            BattleEffect_SetVariant(1);
            BattleSpriteEffect_Spawn(286, position.x, position.y, position.z, 256);
            BattleModelEffect_Spawn(406, 0, position.x, position.y, position.z, 256);
            Overlay10Enemy_PlayHitReaction(actor_id, 407);
            BattleSound_Play(56, 0, 0, 0);
        }
        BattleScreenEffect_StartSecondary(5, 1, 0);
        Overlay10Enemy_AddScaleSteps((Overlay10EnemyState *)work, actor_id, 1);
    } else {
        BattleEffect_SetVariant(-1);
        BattleModelEffect_Spawn(418, 0, position.x, position.y, position.z, 256);
        Overlay10Enemy_PlayHitReaction(actor_id, 408);
        BattleSound_Play(60, 0, 0, 0);
    }
    BattleFeedback_SpawnVariant(object, popup_x, popup_y, 1);
    if (!Overlay10Enemy_IsSelectable(actor_id))
        work->flags |= 2;
}
}
