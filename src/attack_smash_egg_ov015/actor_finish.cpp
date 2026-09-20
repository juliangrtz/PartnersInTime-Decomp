/* Smash Eggs actor finish transition, overlay 15, 0x020C4570-0x020C4728. */
#include "actor_internal.h"
extern "C" {
void Overlay15Attack_BeginActorFinish(Overlay15AttackModelController *state)
{
    Overlay15AttackContext *context;
    BattleSceneObject *object;
    Overlay15AttackModelOwner *owner = state->primary;
    object = owner->object;
    context = data_ov002_020c0710;
    /* Keep the selected model across resource/animation changes below. */
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    u16 hit_actor = (owner->formation_index & 1) ? 9 : 8;
    u32 phase;
    BattleHitDescriptor_DisableByActor(hit_actor);
    BattleHitDescriptor_DisableByActor(40);
    phase = state->phase_flags.bits.phase;
    if (phase == 9) {
        /* Finish the support actor's airborne motion before retreating. */
        BattleSceneObject *secondary = state->hit_motion.object;
        BattleSceneObject_StartScaledAcceleratedMotion(secondary, 2, 0, 0, 1,
            data_ov015_020c5e69[state->primary->formation_index & 1][0] - secondary->z,
            -1024, -128, 1);
        state->phase_flags.raw = (state->phase_flags.raw & ~31) | 11;
        Overlay15Attack_BeginPairRetreat(&context->pair, state);
    } else {
        if (phase >= 1 && phase <= 6) {
            BattleEntity_BindResource(object->actor_id, state->primary_resource_id);
            BattleSceneObject_SetAnimation(object, 5, -1);
            func_0200940c(model, 512);
            Overlay15Attack_ConfigureModelFlags(state);
            state->hit_active = 16;
        } else {
            BattleEntity_BindResource(object->actor_id, state->secondary_resource_id);
            BattleSceneObject_SetAnimation(object, 24, -1);
            func_0200940c(model, 256);
            Overlay15Attack_ConfigureModelFlags(state);
            state->hit_active = 32;
        }
        BattleSceneObject_SetAnimationMode01(object, 0);
        BattleSound_Play(68, 0, 0, 0);
        Overlay15Attack_BeginPairRetreat(&context->pair, state);
        state->phase_flags.raw = (state->phase_flags.raw & ~31) | 7;
    }
    context->flags |= 2;
}

}
