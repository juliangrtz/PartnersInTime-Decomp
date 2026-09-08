#ifndef PIT_GAME_BATTLE_MESH_H
#define PIT_GAME_BATTLE_MESH_H

#include <nitro/fx_mtx.h>

typedef struct BattleMeshVertex {
    s16 x, y, z;
} BattleMeshVertex;
typedef struct BattleMeshCenter {
    s32 x, y, z;
} BattleMeshCenter;
typedef struct BattleMeshTriangle {
    struct BattleMeshTriangle *next;
    const BattleMeshVertex *source[3];
    BattleMeshVertex vertices[3];
    u8 unknown_22[2];
    BattleMeshCenter center;
    u16 color;
    s16 alpha;
    void (*update)(struct BattleMeshTriangle *);
    u8 unknown_38[8];
} BattleMeshTriangle;
typedef char BattleMeshTriangle_SizeCheck[sizeof(BattleMeshTriangle) == 64 ? 1 : -1];

typedef char BattleMeshVertex_SizeCheck[sizeof(BattleMeshVertex) == 6 ? 1 : -1];
typedef char BattleMeshCenter_SizeCheck[sizeof(BattleMeshCenter) == 12 ? 1 : -1];

typedef struct BattleMeshTask BattleMeshTask;
typedef struct BattleMeshResource {
    s16 triangle_count;
    u16 unknown_02;
    BattleMeshTriangle *triangles;
    BattleMeshTask *task;
    MtxFx44 transform;
} BattleMeshResource;
typedef struct BattleMeshTaskState {
    u16 object_id, target_id;
    s16 hits, height, velocity, spin, color_channel;
} BattleMeshTaskState;
struct BattleMeshTask {
    void *next;
    void (*update)(BattleMeshTask *);
    void *previous;
    BattleMeshTaskState state;
};
typedef char BattleMeshResource_SizeCheck[sizeof(BattleMeshResource) == 76 ? 1 : -1];
typedef char BattleMeshTaskState_SizeCheck[sizeof(BattleMeshTaskState) == 14 ? 1 : -1];
typedef char BattleMeshTask_SizeCheck[sizeof(BattleMeshTask) == 28 ? 1 : -1];

struct BattleSceneObject;

#ifdef __cplusplus
extern "C" {
#endif

BattleMeshTask *BattleMesh_CreateHitTask(struct BattleSceneObject *object, int target_id, int hits);
void BattleMesh_UpdateFadeTask(BattleMeshTask *task);
void BattleMesh_RotateAndNormalize(struct BattleSceneObject *object, int axis, int angle, int scale);

BattleMeshCenter *BattleMesh_TransformCenter(BattleMeshCenter *center, const MtxFx44 *matrix);
BattleMeshVertex *BattleMesh_TransformVertex(BattleMeshVertex *vertex, const MtxFx44 *matrix);
BattleMeshTriangle *BattleMesh_SortTriangles(BattleMeshTriangle *triangles, int count, int cull_backfaces);
void BattleMesh_PrepareTriangles(BattleMeshTriangle *triangles, int count, const MtxFx44 *matrix, u16 color,
                                 s16 alpha);

#ifdef __cplusplus
}
#endif
#endif
