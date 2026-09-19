/*
 * Impact effects (overlay 2, 0x020A9C18-0x020AA4C8).
 *
 * The emitters an impact spawns: the trail behind a launched actor, the burst of
 * particles at the point of contact, and the property curve that animates an
 * object's value over the impact.
 */

#include <game/battle_context.h>
#include <game/battle_impact_effect.h>

enum BattleImpactEmitterOffset {
    BATTLE_IMPACT_EMITTER_TASK_POOL_OFFSET = 0x8B44
};
BattleAITask *BattleImpactEmitter_Start(
    u16 object_id, int mode, int target_scale_q4, int lifetime_multiplier,
    int spawn_interval, int size_multiplier, int target_size,
    int initial_z_offset, int z_velocity, int reserved) {
    BattleImpactTrailEmitterTask *task =
        (BattleImpactTrailEmitterTask *)BattleTaskList_Insert(
            (BattleTaskPool *)(gBattleContext +
                               BATTLE_IMPACT_EMITTER_TASK_POOL_OFFSET),
            0);
    BattleImpactTrailEmitterPayload *emitter;

    (void)reserved;

    switch (mode) {
        case 0:
            emitter = &task->data;
            emitter->target_scale_q4 = target_scale_q4;
            emitter->lifetime_multiplier = lifetime_multiplier;
            emitter->setting_bits.spawn_interval = spawn_interval;
            emitter->setting_bits.size_multiplier = size_multiplier;
            emitter->setting_bits.target_size = target_size;
            emitter->initial_z_offset = initial_z_offset;
            emitter->z_velocity = z_velocity;
            emitter->spawn_delay = emitter->setting_bits.spawn_interval;
            emitter->object_id = object_id;
            BattleSceneObject_GetById(object_id)
                ->flags.bits.stop_impact_particles = 0;
            task->callback = emitter->lifetime_multiplier != 0
                                 ? BattleImpactTrailEmitter_Update
                                 : 0;
            break;

        case 1: {
            BattleObjectBurstEmitterPayload *burst_emitter;

            emitter = &task->data;
            emitter->target_scale_q4 = 0;
            emitter->lifetime_multiplier = emitter->target_scale_bits;
            burst_emitter = (BattleObjectBurstEmitterPayload *)emitter;
            burst_emitter->object_id = object_id;
            task->callback = BattleObjectBurstEmitter_Update;
            break;
        }
    }
    return (BattleAITask *)task;
}

void BattleImpactTrailEmitter_Update(BattleAITask *base_task) {
    BattleImpactTrailEmitterTask *task =
        (BattleImpactTrailEmitterTask *)base_task;
    BattleImpactTrailEmitterPayload *emitter = &task->data;
    BattleSceneObject *object = BattleSceneObject_GetById(emitter->object_id);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    BattleImpactParticleTask *particle;
    BattleImpactParticlePayload *particle_data;
    void (*particle_callback)(BattleAITask *task);

    if (model == 0 || !model->flag_bits.animation_active ||
        object->flags.bits.stop_impact_particles) {
        object->flags.bits.stop_impact_particles = 0;
        task->callback = 0;
        return;
    }

    if (emitter->spawn_delay != 0) {
        emitter->spawn_delay--;
    }
    if (emitter->spawn_delay != 0) {
        return;
    }

    emitter->spawn_delay = emitter->setting_bits.spawn_interval;
    particle_callback = object->flags.bits.use_alternate_model
                            ? BattleImpactParticle_UpdateModelFrame
                            : BattleImpactParticle_UpdateResourceFrame;
    particle = BattleImpactParticle_CreateFromObject(object,
                                                     particle_callback);
    particle_data = &particle->data;
    particle_data->controller = emitter;
    particle_data->frame =
        emitter->lifetime_multiplier * emitter->setting_bits.spawn_interval;
    particle_data->render_bits.size =
        (emitter->setting_bits.size_multiplier *
         particle_data->render_bits.size) /
        31;
    particle_data->x += object->property_102;
    particle_data->y += object->property_103;
    particle_data->z += emitter->initial_z_offset;
}

extern s32 _s32_div_f(s32 numerator, s32 denominator);

#define DEFINE_IMPACT_PARTICLE_UPDATE(name, render_particle)                 \
    void name(BattleAITask *base_task) {                                     \
        BattleImpactParticleTask *task =                                    \
            (BattleImpactParticleTask *)base_task;                          \
        BattleImpactParticlePayload *particle = &task->data;                \
        int target_scale_q4;                                                \
        u16 target_size;                                                    \
        int remaining_frames;                                               \
        BattleImpactTrailEmitterPayload *emitter;                           \
                                                                            \
        render_particle(particle);                                          \
        remaining_frames = particle->frame;                                 \
        if (remaining_frames != 0) {                                         \
            emitter = (BattleImpactTrailEmitterPayload *)                   \
                particle->controller;                                       \
            target_scale_q4 = emitter->target_scale_q4;                     \
            particle->scale_x =                                            \
                (target_scale_q4 +                                          \
                 _s32_div_f((remaining_frames - 1) *                        \
                                (16 * particle->scale_x - target_scale_q4),  \
                            remaining_frames)) /                             \
                16;                                                         \
            target_scale_q4 = emitter->target_scale_q4;                     \
            particle->scale_y =                                            \
                (target_scale_q4 +                                          \
                 _s32_div_f((remaining_frames - 1) *                        \
                                (16 * particle->scale_y - target_scale_q4),  \
                            remaining_frames)) /                            \
                16;                                                         \
            target_size = emitter->setting_bits.target_size;                \
            particle->render_bits.size =                                   \
                target_size +                                              \
                _s32_div_f((remaining_frames - 1) *                         \
                               (particle->render_bits.size - target_size),   \
                           remaining_frames);                               \
            particle->z +=                                                 \
                _s32_div_f(emitter->z_velocity,                             \
                           emitter->setting_bits.spawn_interval);           \
            particle->frame--;                                             \
        }                                                                   \
        if (particle->frame == 0) {                                         \
            task->callback = 0;                                             \
        }                                                                   \
    }

DEFINE_IMPACT_PARTICLE_UPDATE(BattleImpactParticle_UpdateResourceFrame,
                              BattleImpactParticle_RenderPrimary)

DEFINE_IMPACT_PARTICLE_UPDATE(BattleImpactParticle_UpdateModelFrame,
                              BattleImpactParticle_RenderAlternate)

#undef DEFINE_IMPACT_PARTICLE_UPDATE

#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_effect.h>
#include <game/battle_scene.h>

enum BattleImpactEffectOffset {
    BATTLE_IMPACT_EFFECT_TASK_POOL_OFFSET = 0x8B44
};
typedef struct BattleObjectPropertyCurvePayload {
    BattleSceneObject *primary_object;
    u16 secondary_object_id;
    s16 amplitude;
    s16 frame;
    s16 duration;
} BattleObjectPropertyCurvePayload;

typedef struct BattleObjectPropertyCurveTask {
    BattleAITask *next;
    void (*callback)(BattleAITask *task);
    BattleAITask **owner_slot;
    BattleObjectPropertyCurvePayload data;
} BattleObjectPropertyCurveTask;

typedef char BattleObjectPropertyCurvePayload_SizeCheck
    [sizeof(BattleObjectPropertyCurvePayload) == 0x0C ? 1 : -1];
typedef char BattleObjectPropertyCurveTask_SizeCheck
    [sizeof(BattleObjectPropertyCurveTask) == 0x18 ? 1 : -1];

void BattleObjectBurstEmitter_Update(BattleAITask *base_task) {
    BattleObjectBurstEmitterTask *task =
        (BattleObjectBurstEmitterTask *)base_task;
    BattleObjectBurstEmitterPayload *data = &task->data;
    BattleSceneObject *object = BattleSceneObject_GetById(data->object_id);
    BattleImpactParticleTask *particle;
    BattleImpactParticlePayload *particle_data;

    if (object->flags.bits.stop_impact_particles) {
        object->flags.bits.stop_impact_particles = 0;
        task->callback = 0;
        return;
    }
    particle = BattleImpactParticle_CreateFromObject(
        object, BattleObjectBurstParticle_Update);
    particle_data = &particle->data;
    particle_data->controller = data;
    particle_data->frame = 0;
}

void BattleObjectBurstParticle_Update(BattleAITask *base_task) {
    BattleImpactParticleTask *task = (BattleImpactParticleTask *)base_task;
    BattleImpactParticlePayload *particle = &task->data;
    u16 size;

    particle->z += 64;
    if (particle->frame == 8) {
        task->callback = 0;
        return;
    }
    size = _s32_div_f(60, particle->frame + 2);
    particle->render_bits.size = size;
    BattleImpactParticle_RenderPrimary(particle);
    particle->frame++;
}

BattleAITask *BattleObjectPropertyCurve_Start(BattleSceneObject *primary_object,
                                              BattleSceneObject *secondary_object,
                                              s16 duration, int amplitude) {
    BattleObjectPropertyCurveTask *task;
    BattleObjectPropertyCurvePayload *data;

    if (primary_object == 0 && secondary_object == 0) {
        return 0;
    }
    task = (BattleObjectPropertyCurveTask *)BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext +
                           BATTLE_IMPACT_EFFECT_TASK_POOL_OFFSET),
        0);
    data = &task->data;
    data->primary_object = primary_object;
    data->secondary_object_id =
        secondary_object != 0 ? secondary_object->actor_id : 0;
    if (amplitude == 0) {
        amplitude = secondary_object->z / 5;
    }
    data->amplitude = amplitude;
    data->duration = duration;
    data->frame = 0;
    task->callback = BattleObjectPropertyCurve_Update;
    return (BattleAITask *)task;
}

void BattleObjectPropertyCurve_Update(BattleAITask *base_task) {
    BattleObjectPropertyCurveTask *task =
        (BattleObjectPropertyCurveTask *)base_task;
    BattleObjectPropertyCurvePayload *data = &task->data;
    int duration = data->duration;
    int amplitude = data->amplitude;
    int phase = 4 * data->frame;
    int duration_squared = duration * duration;
    int previous_value;
    int next_value;
    BattleSceneObject *object;
    int delta;

    if (duration < phase) {
        phase = (duration + 2 * phase) / 4;
    }
    delta = phase - duration;
    previous_value = amplitude -
                     _s32_div_f(amplitude * (delta * delta),
                                duration_squared);

    data->frame++;
    phase = 4 * data->frame;
    if (duration < phase) {
        phase = (duration + 2 * phase) / 4;
    }
    delta = phase - duration;
    next_value = data->amplitude;
    next_value -=
        _s32_div_f(next_value * (delta * delta), duration_squared);

    object = 0;
    if (data->primary_object != 0) {
        object = data->primary_object;
        object->property_103 -= previous_value;
        if (!BattleSceneObject_GetActiveModel(object)
                 ->flag_bits.animation_active) {
            data->primary_object = 0;
        }
    }
    if (data->primary_object != 0) {
        object->property_103 += next_value;
    }

    if (data->secondary_object_id != 0) {
        object = BattleSceneObject_GetById(data->secondary_object_id);
        object->property_103 -= previous_value;
        if (!BattleSceneObject_GetActiveModel(
                 BattleSceneObject_GetById(data->secondary_object_id))
                 ->flag_bits.animation_active) {
            data->secondary_object_id = 0;
        }
    }
    if (data->secondary_object_id != 0) {
        object->property_103 += next_value;
    }

    if ((data->primary_object == 0 && data->secondary_object_id == 0) ||
        (phase != 0 && next_value == 0)) {
        task->callback = 0;
    }
}

int BattleParty_SpawnLaunchImpact(BattleActor *base_actor) {
    BattlePartyActor *actor = (BattlePartyActor *)base_actor;
    s16 position[6];
    u8 animation_id = 16;

    if ((u16)(actor->formation_index + (u16)-2) <= 1) {
        animation_id = 17;
    }
    BattlePosition_StoreViewRelative(
        (BattlePosition *)position, actor->actor.scene_object->x,
        (s16)(actor->actor.scene_object->y - actor->actor.scene_object->z),
        (s16)(actor->actor.scene_object->effect_anchor_z +
              16 * (256 - actor->actor.scene_object->y)),
        actor->actor.scene_object->flags.bits.use_raw_position,
        actor->actor.scene_object->flags.bits.use_alternate_model);
    BattleModelEffect_SpawnFromResource(19, animation_id, position[0],
                                        position[1], position[2], 256);
    return BattleSound_Play(57, 0, 0, 0);
}
