#include "actor_internal.h"

enum Overlay15AttackObjectId {
    OVERLAY15_PRIMARY_OBJECT_ID = 40,
    OVERLAY15_SECONDARY_OBJECT_ID = 42,
};

enum Overlay15AttackObjectPairFlag {
    OVERLAY15_OBJECT_PAIR_FLAG_10 = 0x10,
    OVERLAY15_OBJECT_PAIR_FLAG_20 = 0x20,
    OVERLAY15_OBJECT_PAIR_FLAG_80 = 0x80,
};

extern "C" {

/* Metrowerks emits functions in reverse source order. */
int Overlay15Attack_IsIdle(Overlay15AttackModelController *state)
{
    int result = 1;
    int phase = state->phase_flags.bits.phase;
    if (phase == 0) {
        return result;
    }
    if (phase != 17) {
        result = 0;
    }
    return result;
}

void Overlay15Attack_BeginReverseAnimation(Overlay15AttackModelController *state)
{
    BattleSceneObject *object = state->primary->object;
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    int animation;
    int last_frame;
    BattleSceneObject_SetAnimation(object, 1, -1);
    animation = model->get_animation_id();
    last_frame = func_02009224(model, -1);
    /* Narrow the frame after subtraction, as in the native call. */
    model->set_animation(animation, (s16)(last_frame - 1));
    func_0200940c(model, -state->animation_scale);
    BattleSceneObject_SetAnimationMode01(object, 0);
    BattleSound_Play(57, 0, 0, 0);
    state->phase_flags.raw = (state->phase_flags.raw & ~31) | 2;
}

void Overlay15Attack_BeginSecondaryLaunchCallback(Overlay15AttackModelController *state)
{
    Overlay15Attack_BeginSecondaryLaunch(state);
}

void Overlay15Attack_BeginSecondaryLaunch(Overlay15AttackModelController *state)
{
    Overlay15AttackHitMotion *motion = &state->hit_motion;
    BattleSceneObject *secondary = motion->object;
    BattleSceneObject *primary = state->primary->object;
    BattleSceneObject *follower;
    BattleModel *model;
    int alternate;
    s16 offset_x, offset_y;
    u16 follower_id;
    secondary->effect_anchor_z = -4;
    BattleEntity_BindResource(secondary->actor_id, state->secondary_resource_id);
    BattleSceneObject_SetAnimationFromComponent(secondary, 41, 0, motion->animation_component);
    alternate = state->primary->formation_index & 1;
    BattleSceneObject_AdjustPosition(secondary,
        primary->x - data_ov015_020c5e68[alternate][0] - secondary->x,
        primary->y - secondary->y,
        primary->z + data_ov015_020c5e69[alternate][0] - secondary->z);
    offset_y = data_ov015_020c5e69[alternate][0];
    offset_x = data_ov015_020c5e68[alternate][0];
    model = BattleSceneObject_GetActiveModel(secondary);
    model->animation_offset_x = offset_x;
    model->animation_offset_y = offset_y;
    BattleSceneObject_StartAcceleratedMotionForDuration(secondary, 2, 0, 0, 1,
        0, motion->duration_q8 / 256, motion->terminal_velocity_q8 / 256, 1);
    follower_id = alternate ? 9 : 8;
    follower = BattleSceneObject_GetById(follower_id);
    BattleSceneObject_AdjustPosition(follower,
        secondary->x + data_ov015_020c5e60[alternate][0] - follower->x,
        secondary->y - follower->y,
        secondary->z + data_ov015_020c5e61[alternate][0] - follower->z);
    BattleMotion_StartFollower(secondary, follower, 3);
    BattleEntity_BindResource(primary->actor_id, state->secondary_resource_id);
    BattleSceneObject_SetAnimation(primary, 40, -1);
    Overlay15Attack_ConfigureModelFlags(state);
    state->hit_timer = motion->duration_q8 / 2 / 256;
    state->hit_active = 0;
    BattleSound_Play(41, 0, 0, 0);
    state->phase_flags.raw = (state->phase_flags.raw & ~31) | 9;
}

void Overlay15Attack_UpdateHitWindow(Overlay15AttackModelController *state)
{
    u16 hit_actor = (state->primary->formation_index & 1) ? 9 : 8;
    u16 actor = state->primary->object->actor_id;
    Overlay15AttackHitMotion *motion = &state->hit_motion;
    int window_end, window_start;
    --state->hit_timer;
    window_end = motion->window_end_q8 / 256;
    window_start = motion->window_start_q8 / 256;
    if (!state->hit_active) {
        if (-window_start <= state->hit_timer && state->hit_timer <= window_end) {
            BattleHitDescriptor_Configure(hit_actor, 60, Overlay15Attack_EmptyHitCallback, actor, 63);
            state->hit_active = 1;
        }
    }
    if (state->hit_active) {
        if (-window_start > state->hit_timer) {
            BattleHitDescriptor_DisableByActor(hit_actor);
            state->hit_active = 0;
        }
    }
}

void Overlay15Attack_PrepareSecondaryActor(
    Overlay15AttackModelController *state) {
    BattleSceneObject *secondary = state->hit_motion.object;
    BattleSceneObject *primary = state->primary->object;
    u16 hit_actor_id =
        (state->primary->formation_index & 1) ? 9 : 8;
    BattleHitDescriptor_DisableByActor(hit_actor_id);
    BattleSceneObject_StopMotionChannel(secondary, 3, 0);
    BattleEntity_BindResource(primary->actor_id,
                              state->secondary_resource_id);
    BattleSceneObject_SetAnimation(primary, 42, -1);
    BattleSceneObject_GetActiveModel(primary);
    BattleSceneObject_SetAnimationMode01(primary, 0);
    BattleSceneObject_SetAnimation(secondary, -1, -1);
    Overlay15Attack_ConfigureModelFlags(state);
    state->phase_flags.raw =
        (state->phase_flags.raw & ~0x1F) | 10;
}

void Overlay15Attack_PreparePrimaryActor(
    Overlay15AttackModelController *state) {
    BattleSceneObject *object = state->primary->object;
    BattleModel *model = BattleSceneObject_GetActiveModel(object);

    BattleEntity_BindResource(object->actor_id, state->primary_resource_id);
    BattleSceneObject_SetAnimation(object, 0, -1);
    BattleSceneObject_SetAnimationMode01(object, 1);
    func_0200940c(model, 0x100);
    Overlay15Attack_ConfigureModelFlags(state);
}

void Overlay15Attack_InitializeObjectPair(
    Overlay15AttackObjectPairState *state) {
    state->flags.raw &= ~OVERLAY15_OBJECT_PAIR_FLAG_10;
    state->launch_count = 0;
    state->target_actor_id = 0;
    state->primary = BattleSceneObject_GetById(OVERLAY15_PRIMARY_OBJECT_ID);
    state->secondary = BattleSceneObject_GetById(OVERLAY15_SECONDARY_OBJECT_ID);
    state->flags.raw &= ~OVERLAY15_OBJECT_PAIR_FLAG_20;
    state->pending_damage = 0;
    state->flags.raw |= OVERLAY15_OBJECT_PAIR_FLAG_80;
    state->primary->effect_anchor_z = -16;
}
}
