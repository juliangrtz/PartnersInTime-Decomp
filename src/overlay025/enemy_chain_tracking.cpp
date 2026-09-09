#include "effect_task_internal.h"

extern "C" {
void func_ov025_020c8bf0(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
void Overlay25Enemy_WaitPair(Overlay25Task *task, BattleSceneObject *object, Overlay25WorkPrefix *work)
{
    if (!work->tasks[2].update && !work->tasks[3].update &&
        object->primary_model->flag_bits.panel_animation_trigger) {
        BattleSceneObject_SetAnimation(object, 0, -1);
        task->update = 0;
    }
}
void Overlay25Chain_BeginTracking(Overlay25Task *task, BattleSceneObject *enemy, Overlay25WorkPrefix *work)
{
    Overlay25Parameters *parameters = &task->parameters;
    Overlay25ChainState *chain = &work->chains[task->parameters.index];
    BattleSceneObject *object = BattleSceneObject_GetById(chain->object_id);
    BattleSceneObject_GetById((u16)parameters->parameter);
    chain->x_q8 = (enemy->x + chain->offset_x) << 8;
    chain->y_q8 = (enemy->y + chain->offset_y) << 8;
    chain->z_q8 = (enemy->z + chain->offset_z - enemy->property_103) << 8;
    chain->counter = 0;
    parameters->timer = 0;
    parameters->mode = 0;
    chain->unknown_302 = -1;
    BattleEntity_BindResource(chain->object_id, enemy->resource->object_data_id);
    BattleSceneObject_SetAnimation(object, 30, -1);
    BattleSceneObject_AdjustPosition(object, enemy->x - object->x, enemy->y - object->y,
                                     enemy->z - object->z);
    object->flags.bits.suppress_shadow = 1;
    BattleSceneObject_SnapshotPosition(object);
    BattleHitDescriptor_Configure(chain->object_id, 56, 0, enemy->actor_id, 63);
    task->update = func_ov025_020c8bf0;
    BattleSound_Play(265, 0, 0, 0);
}
}
