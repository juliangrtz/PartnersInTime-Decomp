#include "actor_internal.h"

void Overlay15Attack_HideAttackObjects(Overlay15AttackObjectPairState *state)
{
    BattleSceneObject *third = BattleSceneObject_GetById(43);
    state->primary->effect_anchor_z = 0;
    state->secondary->effect_anchor_z = 0;
    third->effect_anchor_z = 0;
    BattleSceneObject_SetAnimation(state->primary, -1, -1);
    BattleSceneObject_SetAnimation(state->secondary, -1, -1);
    BattleSceneObject_SetAnimation(third, -1, -1);
}

void Overlay15Attack_BeginPairLaunch(Overlay15AttackObjectPairState *state,
    int target_id, int damage, int alternate, int index_offset)
{
    BattleSceneObject *target = BattleSceneObject_GetById(target_id);
    BattleSceneObject *object = state->primary;
    int duration = data_ov015_020c5fdc[
        Overlay15Attack_ClampContextIndex(index_offset)][0];
    int distance;

    Overlay10Motion_StopAll(object);
    ++state->launch_count;
    distance = Overlay10Motion_DistanceXY(object, target, 0, 0);
    duration = duration * distance / 128 / 256;
    BattleSceneObject_MoveTo(object, 2,
        target->x + target->property_0fe,
        target->y + target->property_0ff,
        target->z + target->property_0fa, duration);
    state->flags.raw = (state->flags.raw & ~15) | 2;
    state->flags.raw = (state->flags.raw & ~32) | (((u8)alternate & 1) << 5);
    state->pending_damage = damage;
    state->target_actor_id = target_id;
    state->angle_step = 5242;
    if (target->x < object->x) {
        state->angle_step *= -1;
    }
    BattleSound_Play(0, 0, 0, 0);
}
