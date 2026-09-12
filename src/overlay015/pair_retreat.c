#include "actor_internal.h"

enum { OVERLAY15_VIEW_ORIGIN_X_OFFSET = 0xCB9C };

void Overlay15Attack_BeginPairRetreat(Overlay15AttackObjectPairState *state,
    Overlay15AttackModelController *actor)
{
    BattleSceneObject *object = state->primary;
    int duration = data_ov015_020c5fde[Overlay15Attack_ClampContextIndex(0)][0];
    int distance = data_ov015_020c5fa0[Overlay15Attack_ClampContextIndex(0)] - object->z;
    int x;
    Overlay10Motion_StopAll(object);
    x = *(s16 *)(gBattleContext + OVERLAY15_VIEW_ORIGIN_X_OFFSET);
    BattleSceneObject_StartAcceleratedMotionForDuration(object, 3, 0, 0, 1,
        -object->z, duration, distance, 1);
    if (actor->primary->formation_index & 1) {
        x -= 144;
    } else {
        x += 400;
    }
    BattleSceneObject_MoveTo(object, 2, x, object->y, 0, duration);
    state->flags.raw = (state->flags.raw & ~15) | 4;
    state->flags.raw &= ~32;
    state->pending_damage = 0;
    state->target_actor_id = 0;
}
