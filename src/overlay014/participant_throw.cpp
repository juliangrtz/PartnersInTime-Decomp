#include <game/overlay014_projectile.h>
extern "C" {
#include <game/battle_effect.h>
extern u8 *data_ov002_020c0710;
extern s8 data_ov014_020c6180[][4], data_ov014_020c6204[];
extern int Overlay14Attack_PickTarget(int);
extern void BattleSceneObject_SetAnimationMode01(BattleSceneObject *, int);
extern void func_0200940c(BattleModel *, s16);
extern void func_ov002_0206f1b8(BattleModel *);
extern void Overlay10Motion_StopAll(BattleSceneObject *);
extern void func_ov002_0206b5c0(BattleImpactParticlePayload *);
int Overlay14Attack_HasTarget(void)
{
    return Overlay14Attack_PickTarget(0) != 0;
}

void Overlay14Participant_BeginPrimaryThrow(Overlay14Participant *participant)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    BattleModel *model = BattleSceneObject_GetActiveModel(actor);
    if (Overlay14Attack_HasTarget()) {
        int level = participant->projectile ? participant->projectile->flags.level : 2;
        BattleSceneObject_SetAnimation(actor, 4, -1);
        BattleSceneObject_SetAnimationMode01(actor, 0);
        func_0200940c(model, 576);
        participant->animation_frame = data_ov014_020c6180[participant->actor->formation_index][level >= 2];
        participant->phase = 5;
    }
}

void Overlay14Participant_BeginSecondaryThrow(Overlay14Participant *participant)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    BattleModel *model = BattleSceneObject_GetActiveModel(actor);
    if (Overlay14Attack_HasTarget()) {
        int level = participant->projectile ? participant->projectile->flags.level : 2;
        BattleSceneObject_SetAnimation(actor, level >= 2 ? 7 : 6, -1);
        BattleSceneObject_SetAnimationMode01(actor, 0);
        func_0200940c(model, 512);
        participant->animation_frame =
            data_ov014_020c6180[participant->actor->formation_index][level >= 2 ? 3 : 2];
        participant->phase = 7;
    }
}

void Overlay14Participant_BeginRecovery(Overlay14Participant *participant)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    if (BattleSceneObject_GetActiveModel(actor)->get_animation_id()) {
        BattleSceneObject_SetAnimation(actor, 0, -1);
        BattleSceneObject_SetAnimationMode01(actor, 1);
        func_0200940c(BattleSceneObject_GetActiveModel(actor), 256);
    }
    if (participant->projectile) {
        Overlay14Projectile_BeginHide(participant->projectile);
        participant->projectile = 0;
    }
    participant->phase = 2;
}

void Overlay14Participant_UpdateParticle(BattleAITask *base)
{
    BattleImpactParticleTask *task = (BattleImpactParticleTask *)base;
    BattleImpactParticlePayload *particle = &task->data;
    int frame = ++particle->frame;
    if (frame > 12) {
        task->callback = 0;
        return;
    }
    {
        int alpha = (-23 * frame) / 12 + 24;
        int scale = (192 * frame) / 12 + 256;
        particle->z += 16;
        if (alpha > 0) {
            particle->render_bits.size = alpha;
            particle->scale_x = scale;
            particle->scale_y = scale;
            func_ov002_0206b5c0(particle);
        }
    }
}

void Overlay14Participant_UpdateParticles(Overlay14Participant *participant)
{
    if (--participant->particle_timer <= 0) {
        BattleImpactParticle_CreateFromObject(participant->actor->actor.scene_object,
                                              Overlay14Participant_UpdateParticle);
        participant->particle_timer = 6;
    }
}

void Overlay14Participant_BeginEntry(Overlay14Participant *participant)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    BattleEntity_BindResource(actor->actor_id, participant->resource);
    BattleSceneObject_SetAnimation(actor, 1, -1);
    BattleSound_Play(106, 0, 0, 0);
    participant->timer = 32;
    participant->particle_timer = 0;
    participant->phase = 1;
}

void Overlay14Projectile_Initialize(Overlay14Projectile *projectile, int resource)
{
    projectile->phase = 0;
    projectile->unknown106 = 0;
    projectile->flags.flag0 = 0;
    projectile->flags.level = 0;
    projectile->damage = 0;
    projectile->target_id = 0;
    if (resource) {
        projectile->object.resource = BattleObjectData_GetLoadState(resource);
        projectile->object.animation_variant_offset = 0;
    }
    projectile->object.render_state = 0x7fff;
    projectile->object.effect_anchor_z = -4;
    Overlay10Motion_StopAll(&projectile->object);
}

void Overlay14Projectile_ReleaseModel(BattleSceneObject *object)
{
    if (object->primary_model) {
        func_ov002_0206f1b8(object->primary_model);
        if (object->primary_model)
            object->primary_model->unknown_14();
        object->primary_model = 0;
    }
}

void Overlay14Projectile_Reset(Overlay14Projectile *projectile)
{
    BattleModel *model = BattleSceneObject_GetActiveModel(&projectile->object);
    if (model)
        BattleModel_SetAlpha(model, 31, 0);
    BattleSceneObject_SetAnimation(&projectile->object, -1, -1);
    Overlay14Projectile_Initialize(projectile, 0);
    Overlay10Motion_StopAll(&projectile->object);
}

void Overlay14Projectile_UpdateStopRequest(Overlay14Projectile *projectile)
{
    if (((Overlay14AttackFlags *)(data_ov002_020c0710 + 16))->stop)
        Overlay14Projectile_BeginHide(projectile);
}
}
