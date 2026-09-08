#include <game/battle_mesh.h>
#include <game/battle_scene.h>

void BattleMesh_UpdateFadeTask(BattleMeshTask *task)
{
    unsigned id = task->state.object_id;
    BattleSceneObject *object = BattleSceneObject_GetById(id);
    BattleMeshResource *resource;
    if (!BattleSceneObject_GetActiveModel(object)->flag_bits.animation_active) {
        task->update = 0;
        return;
    }
    object = BattleSceneObject_GetById(id);
    if (object->flags.bits.render_mode != 2) {
        task->update = 0;
        return;
    }
    resource = (BattleMeshResource *)object->resource->data;
    if (resource->task != task) {
        task->update = 0;
        return;
    }
    if (resource->triangles[0].alpha < 1) {
        task->update = 0;
        resource->task = 0;
        object->flags.bits.suppress_shadow = 0;
        BattleSceneObject_SetAnimation(object, -1, -1);
    }
}
