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
