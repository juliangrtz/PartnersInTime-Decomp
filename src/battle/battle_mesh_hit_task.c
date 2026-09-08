#include <game/battle_mesh.h>
#include <game/battle_scene.h>
#include <game/battle_ai.h>

void func_ov002_020bb2d4(BattleMeshTask *task);

BattleMeshTask *BattleMesh_CreateHitTask(BattleSceneObject *object, int target_id, int hits)
{
    BattleMeshTask *task =
        (BattleMeshTask *)BattleTaskList_Insert((BattleTaskPool *)(gBattleContext + 35652), 0);
    BattleMeshResource *resource = (BattleMeshResource *)object->resource->data;
    BattleMeshTaskState *state = &task->state;
    task->state.object_id = object->actor_id;
    state->target_id = target_id;
    state->hits = hits;
    state->height = 0;
    state->velocity = 0;
    state->spin = 0;
    state->color_channel = 0;
    resource->task = task;
    task->update = func_ov002_020bb2d4;
    return task;
}
