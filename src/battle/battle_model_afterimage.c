#include <game/battle_effect.h>
#include <game/battle_impact_effect.h>

enum { BATTLE_MODEL_AFTERIMAGE_TASK_POOL_OFFSET = 46436 };

/* The projection helper writes three halfwords; the caller reserves six bytes. */
typedef struct BattleProjectedPosition {
    s16 x, y, z;
} BattleProjectedPosition;
typedef char BattleProjectedPosition_SizeCheck[sizeof(BattleProjectedPosition) == 6 ? 1 : -1];

void func_ov002_0206ba58(BattleImpactParticlePayload *, BattleModel *, s16, s16, s16);

BattleImpactParticleTask *BattleImpactParticle_CreateFromAlternateObject(BattleSceneObject *object,
                                                                         void (*callback)(BattleAITask *))
{
    BattleModel *model = object->alternate_model;
    BattleImpactParticleTask *task;
    BattleImpactParticlePayload *snapshot;
    BattleProjectedPosition position;
    if (!model)
        return 0;
    task = (BattleImpactParticleTask *)BattleTaskList_Insert(
        (BattleTaskPool *)(gBattleContext + BATTLE_MODEL_AFTERIMAGE_TASK_POOL_OFFSET), 0);
    task->callback = callback;
    /* The alternate model shares the impact particle snapshot layout. */
    snapshot = &task->data;
    snapshot->animation_id = model->animation_id;
    snapshot->model_property_056 = model->property_056;
    snapshot->render_bits.flip_x = model->flag_bits.facing_left;
    snapshot->render_bits.flip_y = model->flag_bits.flip_y;
    snapshot->scale_x = model->scale_x;
    snapshot->scale_y = model->scale_y;
    snapshot->rotation_z_high = (u32)model->rotation_z >> 8;
    snapshot->object = object;
    BattlePosition_StoreViewRelative((BattlePosition *)&position, object->x, (s16)(object->y - object->z),
                                     (s16)(object->effect_anchor_z + 16 * (256 - object->y)), 1, 1);
    snapshot->x = position.x;
    snapshot->y = position.y;
    snapshot->z = position.z;
    snapshot->frame = 0;
    return task;
}
void BattleImpactParticle_UpdateAlternateAfterimage(BattleAITask *base_task)
{
    BattleImpactParticleTask *task = (BattleImpactParticleTask *)base_task;
    BattleImpactParticlePayload *snapshot = &task->data;
    if ((s16)--snapshot->frame >= 0) {
        snapshot->z += 16;
        BattleImpactParticle_RenderAlternate(snapshot);
    } else
        task->callback = 0;
}
void BattleImpactParticle_RenderAlternate(BattleImpactParticlePayload *snapshot)
{
    func_ov002_0206ba58(snapshot, snapshot->object->alternate_model, snapshot->x, snapshot->y, snapshot->z);
}
