#include <game/battle_actor.h>
#include <game/battle_ai.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_impact_effect.h>
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
extern s32 _s32_div_f(s32 numerator, s32 denominator);

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
