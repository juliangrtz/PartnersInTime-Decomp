#include "pair_effects_internal.h"

void Overlay16Participant_StartPhaseOne(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    BattleEntity_BindResource(object->actor_id, (u16)((participant->variant & 1) + 56));
    BattleSceneObject_SetAnimationFromComponent(object, 0, 0, participant->component);
    BattleSceneObject_SetAnimationMode01(object, 0);
    BattleSound_Play(41, 0, 0, 0);
    participant->phase = 1;
}

void Overlay16Participant_StartPhaseThree(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    BattleModel *model;
    BattleEntity_BindResource(object->actor_id, (u16)(participant->variant + 58));
    BattleSceneObject_SetAnimationFromComponent(object, 0, 0, participant->component);
    model = BattleSceneObject_GetActiveModel(object);
    model->flags |= BATTLE_MODEL_FLAG_10;
    model->flags &= ~BATTLE_MODEL_FLAG_11;
    BattleSceneObject_SetAnimationMode01(object, 0);
    participant->phase = 3;
}

void Overlay16Particle_UpdateGrow(BattleAITask *task)
{
    BattleImpactParticlePayload *particle = &((BattleImpactParticleTask *)task)->data;
    int frame = ++particle->frame;
    if (frame > 12)
        task->callback = 0;
    else {
        int size = -23 * frame / 12 + 24;
        int scale = 192 * frame / 12 + 256;
        particle->z += 16;
        if (size > 0) {
            particle->render_bits.size = size;
            particle->scale_x = scale;
            particle->scale_y = scale;
            func_ov002_0206b5c0(particle);
        }
    }
}

void Overlay16Participant_EmitTrail(Overlay16Participant *participant)
{
    if (--participant->timer <= 0) {
        BattleImpactParticle_CreateFromObject(participant->object, Overlay16Particle_UpdateGrow);
        participant->timer = 6;
    }
}

void Overlay16Participant_StartPhaseEight(Overlay16Participant *participant)
{
    BattleSceneObject *object = participant->object;
    BattleEntity_BindResource(object->actor_id, (u16)(participant->variant + 58));
    BattleSceneObject_SetAnimationFromComponent(object, 1, 0, participant->component);
    BattleSceneObject_SetAnimationMode01(object, 1);
    if (participant->variant <= 1) {
        BattleModel *model = BattleSceneObject_GetActiveModel(object);
        model->flags |= BATTLE_MODEL_FLAG_10;
        model->flags &= ~BATTLE_MODEL_FLAG_11;
    }
    BattleSound_Play(106, 0, 0, 0);
    participant->timer = 0;
    participant->timer2 = 32;
    participant->phase = 8;
}
