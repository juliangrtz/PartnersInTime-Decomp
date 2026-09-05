#include <game/battle_context.h>
#include <game/battle_impact_effect.h>

enum BattleImpactParticleSetupOffset {
    BATTLE_IMPACT_PARTICLE_TASK_POOL_OFFSET = 0xB564
};

typedef struct BattleImpactCopyRequest {
    u8 unknown_00[4];
    const void *source;
    void *destination;
    u32 size;
} BattleImpactCopyRequest;

extern void func_0202cc58(const void *source, void *destination, u32 size);
extern void func_0202cd2c(const void *source, void *destination, u32 size);

void BattleImpactCopy_Execute(BattleImpactCopyRequest *request) {
    u32 size = request->size;
    void *destination = request->destination;
    const void *source = request->source;

    if (source < destination) {
        func_0202cd2c(source, destination, size);
    } else {
        func_0202cc58(source, destination, size);
    }
}

BattleImpactParticleTask *BattleImpactParticle_CreateFromObject(
    BattleSceneObject *object, void (*callback)(BattleAITask *task)) {
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    BattleImpactParticleTask *task;
    BattleImpactParticlePayload *particle;

    if (model == 0) {
        return 0;
    }
    task = (BattleImpactParticleTask *)BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext +
                           BATTLE_IMPACT_PARTICLE_TASK_POOL_OFFSET),
        0);
    particle = &task->data;
    particle->animation_id = model->animation_id;
    particle->model_property_056 = model->property_056;
    if (!object->flags.bits.use_alternate_model) {
        particle->render_bits.size = model->animation_state_bits.state;
    }
    particle->render_bits.flip_x = model->flag_bits.facing_left;
    particle->render_bits.flip_y = model->flag_bits.flip_y;
    particle->scale_x = model->scale_x;
    particle->scale_y = model->scale_y;
    particle->rotation_z_high = (u32)model->rotation_z >> 8;
    particle->object = object;
    if (!object->flags.bits.use_alternate_model) {
        particle->x = object->x + model->animation_offset_x;
        particle->y = object->y + model->animation_offset_y - object->z;
        particle->z = object->effect_anchor_z + 16 * (256 - object->y);
    } else {
        particle->x = object->x;
        particle->y = object->y - object->z;
        particle->z = object->effect_anchor_z + 16 * (256 - object->y);
    }
    particle->frame = 0;
    task->callback = callback;
    return task;
}

void BattleImpactParticle_RenderPrimary(BattleImpactParticlePayload *particle) {
    BattleImpactParticle_Render(particle, particle->object->primary_model,
                                particle->x, particle->y, particle->z);
}
