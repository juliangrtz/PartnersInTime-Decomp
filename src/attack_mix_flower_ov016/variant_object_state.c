#include <game/battle_effect.h>
#include <game/battle_object.h>
#include <game/battle_scene.h>
#include <game/overlay016_attack.h>

#include "pair_effects_internal.h"

enum Overlay16AttackObjectConstant {
    OVERLAY16_OBJECT_FIRST_ID = 47,
    OVERLAY16_OBJECT_RESOURCE_ID = 53,
    OVERLAY16_OBJECT_ANIMATION_ID = 9,
    OVERLAY16_OBJECT_MOTION_CHANNEL = 2,
    OVERLAY16_OBJECT_TARGET_Y_OFFSET = 2,
    OVERLAY16_OBJECT_TARGET_SOUND_ID = 301,
    OVERLAY16_OBJECT_PHASE_MASK = 0xF,
};

extern int func_ov002_0206f0bc(BattleSceneObject *object, int argument_1, int argument_2, int argument_3,
                               int argument_4, int argument_5);

void Overlay16Attack_ScheduleParticipant(Overlay16Participant *participant)
{
    Overlay16AttackWork *work = data_ov002_020c0710;
    work->next = participant;
    work->active = 0;
    work->pass_timer = 12;
    Overlay16Attack_BeginObjectTargetMotion((Overlay16AttackObjectState *)&work->linked,
                                            (Overlay16AttackTargetReference *)participant, 12);
}

void Overlay16LinkedObject_Initialize(Overlay16LinkedObject *state)
{
    state->object = BattleSceneObject_GetById(46);
    state->flags &= ~15;
}

void Overlay16LinkedObject_Hide(Overlay16LinkedObject *state)
{
    BattleSceneObject_SetAnimation(state->object, -1, -1);
    state->flags &= ~15;
}

void Overlay16LinkedObject_Update(Overlay16LinkedObject *state)
{
    BattleSceneObject *object = state->object;
    switch (state->bits.phase) {
    case 0:
        break;
    case 1:
        if (!BattleSceneObject_IsAnimationChannelActive(object, 2))
            Overlay16Attack_BeginObjectPhaseTwo((Overlay16AttackObjectState *)state);
        break;
    case 2:
        if (!BattleSceneObject_IsAnimationChannelActive(object, 0)) {
            BattleSceneObject_SetAnimation(object, -1, -1);
            state->flags &= ~15;
        }
        break;
    case 3:
        if (!BattleSceneObject_IsAnimationChannelActive(object, 0)) {
            BattleSceneObject_SetAnimation(object, -1, -1);
            state->flags &= ~15;
        }
        break;
    }
}

void Overlay16Attack_BeginObjectTargetMotion(Overlay16AttackObjectState *state,
                                             Overlay16AttackTargetReference *reference, int duration)
{
    BattleSceneObject *object = state->object;
    BattleSceneObject *target;
    Overlay16AttackTargetParameters *parameters;
    BattleModel *model;

    BattleEntity_BindResource(object->actor_id, OVERLAY16_OBJECT_RESOURCE_ID);
    BattleSceneObject_SetAnimation(object, OVERLAY16_OBJECT_ANIMATION_ID, -1);
    model = BattleSceneObject_GetActiveModel(object);
    BattleModel_SetAlpha(model, 31, 0);
    target = reference->object;
    parameters = &gOverlay16AttackTargetOffsets[reference->flag_bits.offset_variant].parameters;
    BattleSceneObject_MoveTo(object, OVERLAY16_OBJECT_MOTION_CHANNEL, target->x + parameters->x,
                             target->y + OVERLAY16_OBJECT_TARGET_Y_OFFSET, parameters->z, duration);
    BattleSound_Play(OVERLAY16_OBJECT_TARGET_SOUND_ID, 0, 0, 0);
    state->motion_flags = (state->motion_flags & ~OVERLAY16_OBJECT_PHASE_MASK) | 1;
}

void Overlay16Attack_BeginObjectPhaseTwo(Overlay16AttackObjectState *state)
{
    func_ov002_0206f0bc(state->object, 0, -1, 0, 4, 0);
    state->motion_flags = (state->motion_flags & ~OVERLAY16_OBJECT_PHASE_MASK) | 2;
}

void Overlay16Attack_BeginObjectPhaseThree(Overlay16AttackObjectState *state)
{
    func_ov002_0206f0bc(state->object, 0, -1, 0, 4, 0);
    state->motion_flags = (state->motion_flags & ~OVERLAY16_OBJECT_PHASE_MASK) | 3;
}

void Overlay16Attack_LaunchObjectAtReference(Overlay16AttackObjectState *state,
                                             Overlay16AttackTargetReference *reference)
{
    BattleSceneObject *object = state->object;
    BattleSceneObject *target = reference->object;

    BattleSceneObject_AdjustPosition(object, target->x - object->x, target->y - object->y, 64 - object->z);
    Overlay16Attack_BeginObjectTargetMotion(state, reference, 8);
}

void Overlay16Attack_InitializeObjectState(Overlay16AttackObjectState *state, int variant)
{
    state->object = BattleSceneObject_GetById((u16)(variant + OVERLAY16_OBJECT_FIRST_ID));
    state->motion_flags = 0;
    state->control_flag_bits.state = 0;
    state->control_byte = (state->control_byte & ~(3 << 4)) | (((u8)variant & 3) << 4);
    state->control_flag_bits.particle_timer = 0;
}

void Overlay16Attack_ResetObjectState(Overlay16AttackObjectState *state)
{
    if (state->object->resource != 0) {
        BattleSceneObject_SetAnimation(state->object, -1, -1);
    }
    state->control_flag_bits.state = 0;
}

void Overlay16Particle_UpdateShrink(BattleAITask *task)
{
    BattleImpactParticlePayload *particle = &((BattleImpactParticleTask *)task)->data;
    int frame = ++particle->frame;
    if (frame > 6)
        task->callback = 0;
    else {
        int size = -20 * frame / 6 + 21;
        int scale = -128 * frame / 6 + 256;
        particle->z += 16;
        if (size > 0) {
            particle->render_bits.size = size;
            particle->scale_x = scale;
            particle->scale_y = scale;
            func_ov002_0206b5c0(particle);
        }
    }
}

void Overlay16Attack_UpdateEntryObject(Overlay16AttackObjectState *state)
{
    BattleSceneObject *object = state->object;
    switch (state->control_flag_bits.state) {
    case 0:
        break;
    case 1:
        if ((s8)--state->control_flag_bits.particle_timer <= 0) {
            state->particle =
                (BattleAITask *)BattleImpactParticle_CreateFromObject(object, Overlay16Particle_UpdateShrink);
            state->control_flag_bits.particle_timer = 1;
        }
        if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
            func_ov002_0206f0bc(object, 1, -1, 0, 8, 0);
            state->control_flag_bits.state = 2;
        }
        break;
    case 2:
        if (!BattleSceneObject_IsAnimationChannelActive(object, 1)) {
            BattleAITask *particle = state->particle;
            if (!particle || ParticleFinished(particle)) {
                state->motion_flags = 0;
                BattleSceneObject_SetAnimation(object, -1, -1);
                state->control_flag_bits.state = 0;
            }
        }
        break;
    }
}
