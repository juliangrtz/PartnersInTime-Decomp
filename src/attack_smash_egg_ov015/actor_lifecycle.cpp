#include "actor_internal.h"

extern "C" {

/* Metrowerks emits functions in reverse source order. */
void Overlay15Attack_InitializeActorController(Overlay15AttackModelController *state,
    Overlay15AttackModelOwner *owner, int resource_id, int alternate)
{
    state->primary = owner;
    state->secondary_resource_id = owner->object->resource->object_data_id;
    state->primary_resource_id = resource_id;
    state->unknown_0e = 0;
    state->hit_timer = state->unknown_0e;
    state->hit_active = 0;
    state->phase_flags.raw &= ~31;
    state->phase_flags.raw = (state->phase_flags.raw & ~32) | (((u8)alternate & 1) << 5);
    int formation = owner->formation_index;
    state->animation_scale = data_ov015_020c5f1c[formation][0];
    state->unknown_16 = data_ov015_020c5ef8[formation][0];
    state->unknown_18 = data_ov015_020c5f40[formation][0];
    owner->object->effect_anchor_z = 0;
}

void Overlay15Attack_InitializeHitMotion(Overlay15AttackHitMotion *motion,
    BattleSceneObject *object, int animation_component)
{
    motion->object = object;
    motion->animation_component = animation_component;
    motion->terminal_velocity_q8 = 10240;
    motion->duration_q8 = 5632;
    motion->window_end_q8 = 2560;
    motion->window_start_q8 = 1024;
}

void Overlay15Attack_RestoreActors(Overlay15AttackModelController *state)
{
    state->primary->object->effect_anchor_z = 0;
    BattleParty_RestoreIdleAnimation((BattlePartyActor *)state->primary,
        state->primary->object);
    if (state->hit_motion.object != 0) {
        if (BattleSceneObject_GetActiveModel(state->hit_motion.object) != 0) {
            BattleSceneObject_SetAnimation(state->hit_motion.object, -1, -1);
        }
        state->hit_motion.object->effect_anchor_z = 0;
    }
}
}
